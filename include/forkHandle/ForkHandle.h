#ifndef CPP_THREAD_FORKPIPE_H
#define CPP_THREAD_FORKPIPE_H
#include <chrono>
#include <thread>

#include <shout>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <libltdl/lt_system.h>
#include <memory>
#include <functional>

using FileDescriptor = int;

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

class Pipe {
public:
    enum class Indicator : unsigned int {
        Read = 0U, Write = 1U
    };

    Pipe() {
        if (pipe(inputOutputFileDescriptor) == -1) {
            throw std::runtime_error("");
        }
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Read)] = true;
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Write)] = true;
        std::cout << '[' << inputOutputFileDescriptor[0] << ',' << inputOutputFileDescriptor[1] << ']' << std::endl;
    }

    Pipe(const Pipe&) = delete;
    Pipe&operator=(const Pipe&) = delete;

    template<typename Pipe>
    void swap(Pipe &&lhsPipe, Pipe &&rhsPipe)
    {
        std::swap(lhsPipe.hasOwnershipOfFileDescriptorFlag[0], rhsPipe.hasOwnershipOfFileDescriptorFlag[0]);
        std::swap(lhsPipe.inputOutputFileDescriptor[0], rhsPipe.inputOutputFileDescriptor[0]);
        std::swap(lhsPipe.hasOwnershipOfFileDescriptorFlag[1], rhsPipe.hasOwnershipOfFileDescriptorFlag[1]);
        std::swap(lhsPipe.inputOutputFileDescriptor[1], rhsPipe.inputOutputFileDescriptor[1]);
    }

    Pipe(Pipe&& pipe)
    {
        swap(*this, pipe);
    }
    Pipe&operator=(Pipe&& pipe)
    {
        swap(*this, pipe);
        return *this;
    }

    ~Pipe() {
        if (hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Read)]) {
            closePipe(Indicator::Read);
        }
        if (hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(Indicator::Write)]) {
            closePipe(Indicator::Write);
        }
    }

    FileDescriptor passOwnershipOfFileDescriptor(Indicator inputOutputIndicator) {
        hasOwnershipOfFileDescriptorFlag[static_cast<unsigned int>(inputOutputIndicator)] = false;
        return inputOutputFileDescriptor[static_cast<unsigned int>(inputOutputIndicator)];
    }

private:
    void closePipe(Indicator inputOutputIndicator) {
        close(inputOutputFileDescriptor[static_cast<unsigned int>(inputOutputIndicator)]);
    }

    bool hasOwnershipOfFileDescriptorFlag[2] = {false, false};
    FileDescriptor inputOutputFileDescriptor[2];
};

class InputPipe {
public:
    InputPipe(Pipe &&pipe)
            : fileDescriptorOwner(
            std::make_shared<FileDescriptorOwner>(pipe.passOwnershipOfFileDescriptor(Pipe::Indicator::Read))) {
    }

    std::shared_ptr<FileDescriptorOwner> getFileDescriptorOwner() {
        return fileDescriptorOwner;
    }

private:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
};

class OutputPipe {
public:
    OutputPipe(Pipe &&pipe)
            : fileDescriptorOwner(
            std::make_shared<FileDescriptorOwner>(pipe.passOwnershipOfFileDescriptor(Pipe::Indicator::Write))) {
    }

    std::shared_ptr<FileDescriptorOwner> getFileDescriptorOwner() {
        return fileDescriptorOwner;
    }

private:
    std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner;
};

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

class FileDescriptorInputReaderStream : public FileDescriptorStream {
public:
    FileDescriptorInputReaderStream(std::shared_ptr<FileDescriptorOwner> fileDescriptorOwner)
            : FileDescriptorStream(fileDescriptorOwner) {
    }

    FileDescriptorInputReaderStream &operator>>(char &character) {
        std::cout << "Read from: " << fileDescriptor << std::endl;
        if(read(fileDescriptor, &character, 1U) != 1U) {
            throw std::runtime_error("");
        };
        return *this;
    }
};

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

using ProcessFunction = std::function<int(FileDescriptorInputReaderStream,
                                          FileDescriptorOutputWriterStream)>;

struct ParentProcess {
    ParentProcess(
            pid_t child_pid,
            Pipe parentToChildPipe,
            Pipe childToParentPipe)

            : child_pid(child_pid),
              inputReaderStream{FileDescriptorInputReaderStream{
                      InputPipe{std::move(childToParentPipe)}.getFileDescriptorOwner()}},
              outputWriterStream{FileDescriptorOutputWriterStream{
                      OutputPipe{std::move(parentToChildPipe)}.getFileDescriptorOwner()}} {
    }

    ~ParentProcess() {
        kill(child_pid, SIGTERM);
    }

    const pid_t child_pid;

    FileDescriptorInputReaderStream inputReaderStream;
    FileDescriptorOutputWriterStream outputWriterStream;
};

namespace ProcessExecutor {
    bool isChild(pid_t child_pid) {
        return child_pid == 0;
    }

    int doFork() {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            throw std::runtime_error("");
        }

        return child_pid;
    }

    int execute_child_process(
            ProcessFunction processFunction,
            Pipe &&parentToChildPipe,
            Pipe &&childToParentPipe
    ) {
        InputPipe inputPipe{std::move(parentToChildPipe)};
        auto inputReaderStream = FileDescriptorInputReaderStream{inputPipe.getFileDescriptorOwner()};
        OutputPipe outputPipe{std::move(childToParentPipe)};
        auto outputWriterStream = FileDescriptorOutputWriterStream{outputPipe.getFileDescriptorOwner()};

        return processFunction(inputReaderStream, outputWriterStream);
    }

    ParentProcess runForked(ProcessFunction processFunction) {
        Pipe parentToChildPipe;
        Pipe childToParentPipe;

        pid_t child_pid = doFork();

        if (isChild(child_pid)) {
            int returnCode = execute_child_process(processFunction, std::move(parentToChildPipe), std::move(childToParentPipe));
            _exit(returnCode);
        } else {
            return ParentProcess(child_pid, std::move(parentToChildPipe), std::move(childToParentPipe));
        }
    }

}

#endif //CPP_THREAD_FORKPIPE_H
