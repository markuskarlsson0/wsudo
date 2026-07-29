#include "ipc/data/data.h"
#include "ipc/pipe/client.h"
#include "ipc/pipe/host.h"
#include <Windows.h>
#include <atomic>
#include <exception>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <thread>

namespace {

class Connection {
  public:
    Connection() : host_(pipeName_) {
        std::exception_ptr clientError;

        std::thread clientThread([this, &clientError] {
            try {
                client_ = std::make_unique<ipc::pipe::Client>(pipeName_);
            } catch (...) {
                clientError = std::current_exception();
            }
        });

        host_.waitForConnection();
        clientThread.join();

        if (clientError) {
            std::rethrow_exception(clientError);
        }
    }

    ipc::pipe::Host& host() { return host_; }
    ipc::pipe::Client& client() { return *client_; }

  private:
    std::wstring pipeName_;
    ipc::pipe::Host host_;
    std::unique_ptr<ipc::pipe::Client> client_;
};

} // namespace

TEST(PipeTest, HostGeneratesUniqueNames) {
    std::wstring first;
    std::wstring second;

    ipc::pipe::Host firstHost(first);
    ipc::pipe::Host secondHost(second);

    EXPECT_NE(first, second);
}

TEST(PipeTest, ClientConnectsToHost) {
    EXPECT_NO_THROW({ Connection connection; });
}

TEST(PipeTest, HostSendsDataToClient) {
    Connection connection;
    DWORD processId = GetCurrentProcessId();

    connection.host().send(ipc::data::Data(processId, L"cmd.exe /c echo test"));
    ipc::data::Data received = connection.client().receive();

    EXPECT_EQ(received.processId, processId);
    EXPECT_EQ(std::wstring(received.command), L"cmd.exe /c echo test");
}

TEST(PipeTest, HostSendsEmptyCommandToClient) {
    Connection connection;

    connection.host().send(ipc::data::Data(10, L""));
    ipc::data::Data received = connection.client().receive();

    EXPECT_EQ(received.processId, static_cast<DWORD>(10));
    EXPECT_EQ(received.command[0], L'\0');
}

TEST(PipeTest, HostSendsMaxLengthCommandToClient) {
    Connection connection;
    std::wstring command(ipc::data::commandSize - 1, L'a');

    connection.host().send(ipc::data::Data(1, command));
    ipc::data::Data received = connection.client().receive();

    EXPECT_EQ(std::wstring(received.command), command);
}

TEST(PipeTest, HostSendsManyMessagesToClientInARow) {
    Connection connection;
    int messageCount = 1000;

    for (int i = 0; i < messageCount; i++) {
        std::wstring command = L"cmd.exe /c echo " + std::to_wstring(i);

        connection.host().send(ipc::data::Data(static_cast<DWORD>(i), command));
        ipc::data::Data received = connection.client().receive();

        ASSERT_EQ(received.processId, static_cast<DWORD>(i));
        ASSERT_EQ(std::wstring(received.command), command);
    }
}

TEST(PipeTest, ClientSendsDataToHost) {
    Connection connection;

    connection.client().send(ipc::data::Data(20, L"from client"));
    ipc::data::Data received = connection.host().receive();

    EXPECT_EQ(received.processId, static_cast<DWORD>(20));
    EXPECT_EQ(std::wstring(received.command), L"from client");
}

TEST(PipeTest, SendsDataConcurrentlyInBothDirections) {
    Connection connection;
    int messageCount = 1000;
    std::atomic<int> mismatches{0};

    std::thread hostToClient([&] {
        for (int i = 0; i < messageCount; i++) {
            std::wstring command = L"host to client " + std::to_wstring(i);

            connection.host().send(ipc::data::Data(static_cast<DWORD>(i), command));
            ipc::data::Data received = connection.client().receive();

            if (received.processId != static_cast<DWORD>(i) ||
                std::wstring(received.command) != command) {
                mismatches++;
            }
        }
    });

    std::thread clientToHost([&] {
        for (int i = 0; i < messageCount; i++) {
            std::wstring command = L"client to host " + std::to_wstring(i);

            connection.client().send(ipc::data::Data(static_cast<DWORD>(i), command));
            ipc::data::Data received = connection.host().receive();

            if (received.processId != static_cast<DWORD>(i) ||
                std::wstring(received.command) != command) {
                mismatches++;
            }
        }
    });

    hostToClient.join();
    clientToHost.join();

    EXPECT_EQ(mismatches.load(), 0);
}
