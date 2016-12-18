// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTOROWNER_H
#define CPP_THREAD_FILEDESCRIPTOROWNER_H

#include "FileDescriptor.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

class FileDescriptorOwner {
public:
    FileDescriptorOwner(FileDescriptor fileDescriptor)
            : fileDescriptor(fileDescriptor) {
    }

    FileDescriptorOwner(const FileDescriptorOwner &) = delete;

    FileDescriptorOwner &operator=(const FileDescriptorOwner &) = delete;

    template<typename FileDescriptorOwner>
    void swap(FileDescriptorOwner &&lhsFileDescriptorOwner, FileDescriptorOwner &&rhsFileDescriptorOwner) {
        std::swap(lhsFileDescriptorOwner.fileDescriptor, rhsFileDescriptorOwner.fileDescriptor);
    }

    FileDescriptorOwner(FileDescriptorOwner &&fileDescriptor) {
        swap(*this, fileDescriptor);
    }

    FileDescriptorOwner &operator=(FileDescriptorOwner &&fileDescriptor) {
        swap(*this, fileDescriptor);
        return *this;
    }

    FileDescriptor get() {
        return fileDescriptor;
    }


    ~FileDescriptorOwner() {
        close(fileDescriptor);
    }

private:
    FileDescriptor fileDescriptor;
};

#endif //CPP_THREAD_FILEDESCRIPTOROWNER_H
