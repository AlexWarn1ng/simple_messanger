#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <msclr/marshal_cppstd.h>
#pragma comment(lib, "ws2_32.lib")
#pragma once

namespace clientPart {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//
		}

	protected:
		/// <summary>
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	protected:
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::RichTextBox^ richTextBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Panel^ panel1;

	private:
		/// <summary>
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(271, 286);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 102);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Send";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(13, 286);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(252, 102);
			this->richTextBox1->TabIndex = 1;
			this->richTextBox1->Text = L"";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(13, 46);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->Size = System::Drawing::Size(333, 234);
			this->richTextBox2->TabIndex = 2;
			this->richTextBox2->Text = L"";
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox1->Location = System::Drawing::Point(71, 20);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 3;
			this->textBox1->Tag = L"";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(177, 20);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(46, 20);
			this->textBox2->TabIndex = 4;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(14, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(25, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"IP /";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(34, 23);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(37, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"PORT";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(229, 18);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"CONNECT";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Red;
			this->panel1->Location = System::Drawing::Point(310, 18);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(23, 22);
			this->panel1->TabIndex = 9;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(357, 400);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private:
	SOCKET client_socket;
	bool isConnected = false;

public:
	System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		// intiate Winsock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			richTextBox2->AppendText("WSAStartup failed\r\n");
			return;
		};
		// recieving from TextBox
		std::string server_ip = msclr::interop::marshal_as<std::string>(textBox1->Text);
		int server_port;
		if (!Int32::TryParse(textBox2->Text, server_port)) {
			richTextBox2->AppendText("Invalid port number\r\n");
			return;
		}
		// create socket
		client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (client_socket == INVALID_SOCKET) {
			richTextBox2->AppendText("Socket creation error\r\n");
			WSACleanup();
			return;
		}
		// configure adress
		sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(server_port);

		if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
			richTextBox2->AppendText("Invalid IP address\r\n");
			closesocket(client_socket);
			WSACleanup();
			return;
		}

		// connecting
		richTextBox2->AppendText("Connecting to server...\r\n");
		if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
			richTextBox2->AppendText("Connect failed\r\n");
			closesocket(client_socket);
			WSACleanup();
			return;
		}

		richTextBox2->AppendText("Connected to server!\r\n\n");
		panel1->BackColor = System::Drawing::Color::Green;
		isConnected = true;

		// creating a stream for recieving messages
		System::Threading::Thread^ receiverThread = gcnew System::Threading::Thread(
			gcnew System::Threading::ThreadStart(this, &MyForm::ReceiveMessagesThread));
		receiverThread->Start();
	}

public:
	System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!isConnected) {
			richTextBox2->AppendText("Not connected to server!\r\n");
			return;
		}

		String^ messageText = richTextBox1->Text;
		if (String::IsNullOrEmpty(messageText)) {
			richTextBox2->AppendText("Message is empty!\r\n");
			return;
		}

		std::string message = msclr::interop::marshal_as<std::string>(messageText);

		if (message == "exit") {
			closesocket(client_socket);
			WSACleanup();
			isConnected = false;
			richTextBox2->AppendText("Disconnected from server by command\r\n");
			panel1->BackColor = System::Drawing::Color::Red;
			return;
		}

		richTextBox2->AppendText("Sent: " + messageText + "\r\n");

		int result = send(client_socket, message.c_str(), message.size(), 0);
		if (result == SOCKET_ERROR) {
			richTextBox2->AppendText("Send failed\r\n");
		}
	}

private:
	void ReceiveMessagesThread() {
		char buffer[1024];
		while (isConnected) {
			int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
			if (bytesReceived <= 0) {
				this->Invoke(gcnew Action<String^>(this, &MyForm::AppendToChatBox),
					"Disconnected from server\r\n");
				panel1->BackColor = System::Drawing::Color::Red;
				isConnected = false;
				break;
			}

			std::string message(buffer, bytesReceived);
			this->Invoke(gcnew Action<String^>(this, &MyForm::AppendToChatBox),
				"Received: " + gcnew String(message.c_str()) + "\r\n");
		}
	}

private:
	void AppendToChatBox(String^ message) {
		richTextBox2->AppendText(message);  
	}


};
}

