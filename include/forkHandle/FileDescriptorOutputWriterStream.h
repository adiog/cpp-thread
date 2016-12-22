// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H
#define CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H

#include <stdio.h>


class FileDescriptorOutputWriterStream : public FileDescriptorStream {
public:
    FileDescriptorOutputWriterStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner)
            : FileDescriptorStream(fileDescriptorOwner, "w")
    {
    }

    FileDescriptorStream &operator<<(const char &character) {
        if (write(fileDescriptor, &character, 1U) != 1U) {
            throw std::runtime_error("");
        }
        return *this;
    }

    FileDescriptorStream &operator<<(const int &integer) {
        fprintf(file, "%d", integer);
        /*if (
         == -1) {
            throw std::runtime_error("");
        }*/
        return *this;
    }

};


#endif //CPP_THREAD_FILEDESCRIPTOROUTPUTWRITERSTREAM_H
