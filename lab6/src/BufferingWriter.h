#ifndef YADRO_BUFFERING_WRITER_H
#define YADRO_BUFFERING_WRITER_H

#include <memory>
#include <string>

struct Record;

/// a writer for multi-threaded writing of data to a file
class BufferingWriter {
public:
    /// Constructor
    /// @param[in] filename file name to write to
    explicit BufferingWriter(std::string const& filename);

    /// Copy constructor. Deleted
    BufferingWriter(const BufferingWriter& other) = delete;

    /// Copy assignment. Deleted
    BufferingWriter& operator=(const BufferingWriter& other) = delete;

    /// Move constructor. Deleted
    BufferingWriter(BufferingWriter&& other) = delete;

    /// Move assignment. Deleted
    BufferingWriter& operator=(const BufferingWriter&& other) = delete;

    /// Destructor
    ~BufferingWriter();

    /// Write record to file
    /// @param[in] record the record to add to the file
    void Write(const Record& record);

private:
    struct BufferingWriterImpl;
    std::unique_ptr<BufferingWriterImpl> impl_; ///< class implementation

    /// Get const pointer to the implementation
    /// @return const pointer to the implementation
    [[nodiscard]] const BufferingWriterImpl* Pimpl() const { return impl_.get(); }

    /// Get pointer to the implementation
    /// @return pointer to the implementation
    [[nodiscard]] BufferingWriterImpl* Pimpl() { return impl_.get(); }
};

#endif