// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTORINPUTREADERSTREAM_H
#define CPP_THREAD_FILEDESCRIPTORINPUTREADERSTREAM_H

#include <memory>
#include "FileDescriptorStream.h"

class FileDescriptorInputReaderStream : public FileDescriptorStream {
public:
    FileDescriptorInputReaderStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner)
            : FileDescriptorStream(fileDescriptorOwner) {
    }

    FileDescriptorInputReaderStream &operator>>(char &character) {
        if(read(fileDescriptor, &character, 1U) != 1U) {
            throw std::runtime_error("");
        };
        return *this;
    }
};

#endif //CPP_THREAD_FILEDESCRIPTORINPUTREADERSTREAM_H
