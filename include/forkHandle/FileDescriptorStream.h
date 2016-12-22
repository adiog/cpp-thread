// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTORSTREAM_H
#define CPP_THREAD_FILEDESCRIPTORSTREAM_H

#include "forkHandle/FileDescriptor.h"
#include "forkHandle/FileDescriptorOwner.h"


class FileDescriptorStream {
public:
    FileDescriptorStream() = default;

    FileDescriptorStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner, const char *filePermissions)
            : fileDescriptorOwner(fileDescriptorOwner)
            , fileDescriptor(fileDescriptorOwner->get())
            , file(fdopen(fileDescriptor, filePermissions))
    {
        if (file == 0) {
            throw std::logic_error("");
        }
    }

protected:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
    FileDescriptor fileDescriptor;
    FILE *file;
};

#endif //CPP_THREAD_FILEDESCRIPTORSTREAM_H
