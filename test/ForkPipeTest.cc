#include <gtest/gtest.h>
#include <forkHandle/ForkHandle.h>
#include <forkHandle/ForkHandleRunner.h>
#include "forkHandle/FileDescriptorOwner.h"
#include "forkHandle/Pipe.h"
#include "forkHandle/FileDescriptorInputReaderStream.h"
#include "forkHandle/FileDescriptorOutputWriterStream.h"
#include <chrono>
#include <thread>


TEST(ForkPipeTestSuite, FileDescriptorOwner)
{
    {
        auto fileDescriptorOwner = FileDescriptorOwner{13};
        ASSERT_EQ(13, fileDescriptorOwner.get());
    }

    {
        auto fileDescriptorOwner = FileDescriptorOwner{14};
        auto movedFileDescriptorOwner = std::move(fileDescriptorOwner);
        ASSERT_EQ(movedFileDescriptorOwner.get(), 14);
    }

    {
        auto fileDescriptorOwner = FileDescriptorOwner{15};
        auto fileDescriptorOwnerMock = FileDescriptorOwner{16};
        fileDescriptorOwnerMock = std::move(fileDescriptorOwner);
        ASSERT_EQ(fileDescriptorOwnerMock.get(), 15);
    }
}

TEST(ForkPipeTestSuite, Pipe)
{
    Pipe pipe();

}


int unsafe_process(FileDescriptorInputReaderStream inputReaderStream, FileDescriptorOutputWriterStream outputWriterStream) {
    char roundrobin = '_';

    inputReaderStream >> roundrobin;

    std::cout << "Child: read " << roundrobin << std::endl;

    roundrobin = 'b';

    outputWriterStream << roundrobin;

    std::cout << "Child: written " << roundrobin << std::endl;

    inputReaderStream >> roundrobin;

    std::cout << "Child: read " << roundrobin << std::endl;

    roundrobin = 'd';

    outputWriterStream << roundrobin;

    std::cout << "Child: written " << roundrobin << std::endl;

    return 12;
}

TEST(ForkPipeTestSuite, MainTest)
{

    ForkHandle parentProcess = ForkHandleRunner::runForked(unsafe_process);


    char roundrobin = 'a';
    parentProcess.outputWriterStream << roundrobin;
    std::cout << "Parent: written " << roundrobin << std::endl;

    parentProcess.inputReaderStream >> roundrobin;
    std::cout << "Parent: read " << roundrobin << std::endl;
    ASSERT_EQ(roundrobin, 'b');

    roundrobin = 'c';
    parentProcess.outputWriterStream << roundrobin;
    std::cout << "Parent: written " << roundrobin << std::endl;

    parentProcess.inputReaderStream >> roundrobin;
    std::cout << "Parent: read " << roundrobin << std::endl;

    ASSERT_EQ(roundrobin, 'd');
}

int read_char(
    FileDescriptorInputReaderStream inputReaderStream,
    FileDescriptorOutputWriterStream outputWriterStream)
{
    char roundrobin = -1;

    inputReaderStream >> roundrobin;

    std::cout << "Child: read " << roundrobin << std::endl;

    return 13;
}

TEST(ForkPipeTestSuite, ParentWriteChar)
{
    ForkHandle parentProcess = ForkHandleRunner::runForked(read_char);

    char roundrobin = 'x';
    parentProcess.outputWriterStream << roundrobin;
    std::cout << "Parent: written " << roundrobin << std::endl;

}

int write_char(
        FileDescriptorInputReaderStream inputReaderStream,
        FileDescriptorOutputWriterStream outputWriterStream)
{
    char roundrobin = 'y';

    outputWriterStream << roundrobin;

    std::cout << "Child: written " << roundrobin << std::endl;

    return 14;
}


TEST(ForkPipeTestSuite, ParentReadChar)
{
    ForkHandle parentProcess = ForkHandleRunner::runForked(write_char);
    char roundrobin = -1;
    parentProcess.inputReaderStream >> roundrobin;
    std::cout << "Parent: read " << roundrobin;
}
