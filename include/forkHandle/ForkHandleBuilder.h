// This file is a part of cpp-thread project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

#ifndef CPP_THREAD_FORKHANDLEBUILDER_H
#define CPP_THREAD_FORKHANDLEBUILDER_H

class ForkHandleBuilder{
    static bool isChild(pid_t child_pid) {
        return child_pid == 0;
    }

    static int doFork() {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            throw std::runtime_error("");
        }

        return child_pid;
    }

    static int execute_child_process(
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

    static ForkHandle runForked(ProcessFunction processFunction) {
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
};


#endif //CPP_THREAD_FORKHANDLEBUILDER_H
