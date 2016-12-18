// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H
#define CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H
class FileDescriptorOutputWriterStream : public FileDescriptorStream {
public:
    FileDescriptorOutputWriterStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner)
            : FileDescriptorStream(fileDescriptorOwner) {
    }

    FileDescriptorStream &operator<<(const char &character) {
        std::cout << "Write to: " << fileDescriptor << std::endl;
        if (write(fileDescriptor, &character, 1U) != 1U) {
            throw std::runtime_error("");
        }
        return *this;
    }

};


#endif //CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H
