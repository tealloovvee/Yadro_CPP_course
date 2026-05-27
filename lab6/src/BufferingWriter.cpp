#include <atomic>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>

#include "BufferingWriter.h"
#include "Record.h"

/// Implementation of the BufferingWriter
struct BufferingWriter::BufferingWriterImpl {
    /// Constructor
    /// @param[in] filename file name to write to
    explicit BufferingWriterImpl(const std::string& path) : file_(path) {
        flusher_ = std::thread(&BufferingWriter::BufferingWriterImpl::RunFlusher, this);
    }

    /// Write record to file
    /// @param[in] record the record to add to the file
    void Write(const Record& record);

    /// Copy constructor. Deleted
    BufferingWriterImpl(const BufferingWriterImpl& other) = delete;

    /// Copy assignment. Deleted
    BufferingWriterImpl& operator=(const BufferingWriterImpl& other) = delete;

    /// Move constructor. Deleted
    BufferingWriterImpl(BufferingWriterImpl&& other) = delete;

    /// Move assignment. Deleted
    BufferingWriterImpl& operator=(const BufferingWriterImpl&& other) = delete;

    /// Destructor
    ~BufferingWriterImpl();

private:
    std::thread flusher_;           ///< a worker saving results to a file
    std::ofstream file_;            ///< file stream
    std::mutex mtx_;                ///< mutex
    std::condition_variable cv_;    ///< condition variable
    std::atomic<bool> run_{true};   ///< a flag indicating that a thread has finished running.
    std::queue<Record> pending_;    ///< queue to pushing records
    std::queue<Record> processing_; ///< queue to processing records

    /// Thread's main job
    void RunFlusher();

    /// Save all records from the processing queue and free it
    void StoreData();
};

void BufferingWriter::Write(const Record& record) { Pimpl()->Write(record); }

void BufferingWriter::BufferingWriterImpl::Write(const Record& record) {
    {
        const std::lock_guard<std::mutex> lock(mtx_);
        pending_.push(record);
    }
    cv_.notify_one();
}

BufferingWriter::BufferingWriterImpl::~BufferingWriterImpl() {
    run_ = false;
    cv_.notify_one();
    flusher_.join();
}

void BufferingWriter::BufferingWriterImpl::RunFlusher() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]() { return !pending_.empty() || !run_; });
        if (!run_) {
            std::swap(pending_, processing_);
            lock.unlock();
            StoreData();
            break;
        }
        std::swap(pending_, processing_);
        lock.unlock();
        StoreData();
    }
}

void BufferingWriter::BufferingWriterImpl::StoreData() {
    while (!processing_.empty()) {
        file_ << processing_.front().data_ << '\n';
        processing_.pop();
    }
}

BufferingWriter::BufferingWriter(const std::string& filename)
    : impl_(std::make_unique<BufferingWriterImpl>(filename)){};

BufferingWriter::~BufferingWriter() = default;