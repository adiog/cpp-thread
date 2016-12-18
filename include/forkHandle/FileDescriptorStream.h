// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTORSTREAM_H
#define CPP_THREAD_FILEDESCRIPTORSTREAM_H

#include "forkHandle/FileDescriptor.h"
#include "forkHandle/FileDescriptorOwner.h"


class FileDescriptorStream {
public:
    FileDescriptorStream() = default;

    FileDescriptorStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner)
            : fileDescriptorOwner(fileDescriptorOwner), fileDescriptor(fileDescriptorOwner->get()) {
    }

protected:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
    FileDescriptor fileDescriptor;
};

#endif //CPP_THREAD_FILEDESCRIPTORSTREAM_H
