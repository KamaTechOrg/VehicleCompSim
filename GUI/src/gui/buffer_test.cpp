//
// Created by OWNER on 15/09/2024.
//

#include "buffer_test.h"
#include "state/globalstate.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

buffer_test::buffer_test(QObject *parent) : QObject(parent), m_logFile(QDir::currentPath() + "/A.log"), msg_counter(0) {
//    if (m_logFile.exists()) {
//        if (!m_logFile.remove()) {
//            qDebug() << "Could not delete existing log file:" << m_logFile.errorString();
//            return;
//        }
//    }
//    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        qWarning() << "Error opening the log file for reading and writing.";
//        return;
//    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &buffer_test::my_new_buffer);
    m_timer->start(1000);
}
buffer_test::~buffer_test() {
//    if (m_logFile.isOpen()) {
//        m_logFile.close();
//    }
//    delete m_timer;
}

void buffer_test::generate_buffer() {
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    int choosen_src = rand() % 4 + 1;
    int choosen_dest = rand() % 4 + 1;
    while (choosen_dest == choosen_src) {
        choosen_dest = rand() % 4 + 1;
    }
    QByteArray buffer;
    buffer.resize(100);
    buffer.fill(0); // Fill the entire buffer with zeros
    if (choosen_src == 1) {
        int index = 0;
        for (int i = 0; i < 20; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
//        QByteArray extractedBytes = buffer.mid(0, 20);
//        QString resultString = QString::fromLatin1(extractedBytes);
//        qInfo() << "Resulting string:" << resultString;

        ////////////////////////////////////
        int num2 = rand() % 200 + 1;
//        qInfo() << "sizeof(int)" << sizeof(int);
//        qInfo() << "sizeof(double)" << sizeof(double);
//        qInfo() << "num2" << num2;
        QDataStream writeStream(&buffer, QIODevice::WriteOnly);
        writeStream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
        writeStream.device()->seek(index);  // Move to index 20
        writeStream << num2;
//        int result;
//        QDataStream stream(buffer.mid(index, sizeof(int)));
//        stream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
//        stream >> result;
//        qDebug() << "result int value:" << result;
        index+= 4;
        ////////////////////////////
        double numDouble = rand() % 200 + 1;
//        qInfo() << "double num3" << numDouble;

        QDataStream doubleWriteStream(&buffer, QIODevice::WriteOnly);
        doubleWriteStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
        doubleWriteStream.device()->seek(index); // Move to index 20
        doubleWriteStream << numDouble;

//        double resultDouble;
//        QDataStream doubleReadStream(buffer.mid(index, sizeof(double)));
//        doubleReadStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
//        doubleReadStream >> resultDouble;
//        qDebug() << "Result double value:" << resultDouble;
        index += 8;
        /////////////////////////////

        int ddd = index;
//        qInfo() << "index" << index;
//        qInfo() << "ddd" << ddd;
        for (int i = 0; i < 30; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
//        qInfo() << "ddd" << ddd;
//        qInfo() << "ddd+30" << ddd + 30;

//        QByteArray extractedBytes1 = buffer.mid(ddd, ddd + 30);
//        QString resultString1 = QString::fromLatin1(extractedBytes1);
//        qInfo() << "Resulting string1:" << resultString1;


    } else if (choosen_src == 2) {
        int index = 0;
        ////////////////
        int num2 = rand() % 200 + 1;
//        qInfo() << "sizeof(int)" << sizeof(int);
//        qInfo() << "sizeof(double)" << sizeof(double);
//        qInfo() << "num2" << num2;
        QDataStream writeStream(&buffer, QIODevice::WriteOnly);
        writeStream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
        writeStream.device()->seek(index);  // Move to index 20
        writeStream << num2;
//        int result;
//        QDataStream stream(buffer.mid(index, sizeof(int)));
//        stream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
//        stream >> result;
//        qDebug() << "result int value:" << result;
        index+= 4;
        ///////////////
        double numDouble = rand() % 200 + 1;
//        qInfo() << "double num3" << numDouble;

        QDataStream doubleWriteStream(&buffer, QIODevice::WriteOnly);
        doubleWriteStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
        doubleWriteStream.device()->seek(index); // Move to index 20
        doubleWriteStream << numDouble;

//        double resultDouble;
//        QDataStream doubleReadStream(buffer.mid(index, sizeof(double)));
//        doubleReadStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
//        doubleReadStream >> resultDouble;
//        qDebug() << "Result double value:" << resultDouble;
        index += 8;
        ///////////////
//        double num3 = rand() % 200 + 1;
//        buffer[index] = num3;
//        index++;
        for (int i = 0; i <= 20; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
        for (int i = 0; i <= 30; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
    } else if (choosen_src == 3) {
        int index = 0;
        //////////////////////
        double numDouble = rand() % 200 + 1;
//        qInfo() << "double num3" << numDouble;

        QDataStream doubleWriteStream(&buffer, QIODevice::WriteOnly);
        doubleWriteStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
        doubleWriteStream.device()->seek(index); // Move to index 20
        doubleWriteStream << numDouble;

//        double resultDouble;
//        QDataStream doubleReadStream(buffer.mid(index, sizeof(double)));
//        doubleReadStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
//        doubleReadStream >> resultDouble;
//        qDebug() << "Result double value:" << resultDouble;
        index += 8;
        ///////////////////////
//        double num3 = rand() % 200 + 1;
//        buffer[index] = num3;
//        index++;
        for (int i = 0; i <= 20; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
        ///////////////////////
        int num2 = rand() % 200 + 1;
//        qInfo() << "sizeof(int)" << sizeof(int);
//        qInfo() << "sizeof(double)" << sizeof(double);
//        qInfo() << "num2" << num2;
        QDataStream writeStream(&buffer, QIODevice::WriteOnly);
        writeStream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
        writeStream.device()->seek(index);  // Move to index 20
        writeStream << num2;
//        int result;
//        QDataStream stream(buffer.mid(index, sizeof(int)));
//        stream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
//        stream >> result;
//        qDebug() << "result int value:" << result;
        index+= 4;
        //////////////////////
//        int num2 = rand() % 200 + 1;
//        buffer[index] = num2;
//        index++;
        for (int i = 0; i <= 30; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
    } else if (choosen_src == 4) {
        int index = 0;
        for (int i = 0; i <= 20; ++i) {
            char randomChar = static_cast<char>(rand() % 26 + 'A'); // Random uppercase letter
            buffer[index] = randomChar;
            index++;
        }
    }
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);

    // Write header fields
    stream << timestamp.toUtf8() << ','
           << QByteArray::number(choosen_src) << ','
           << QByteArray::number(choosen_dest) << ','
           << QByteArray::number(buffer.size()) << ',';
    stream << buffer;
    QList<QByteArray> pieces = message.split(',');
//    for (const auto& item : pieces) {
//        qInfo() << "Raw bytes:" << item.toHex();
//    }
//
//    for(const auto& item : pieces){
//        qInfo() << item;
//    }


    GlobalState::getInstance().new_test_buffer(message);

}

void buffer_test::my_new_buffer() {
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::gmtime(&itt), "%Y-%m-%dT%H:%M:%S");
    std::string timestamp = ss.str();

//    GlobalState::getInstance().currentProject()->models()

    int src = 3;
    int dest = 4;
    int buffer_len = 200;

    char buffer[200];
    int index = 0;

    // Serialize int
    int age = 5678;
    std::memcpy(buffer + index, &age, sizeof(age));
    index += sizeof(age);

    // Serialize double
    double high = 4.555;
    std::memcpy(buffer + index, &high, sizeof(high));
    index += sizeof(high);

    // Serialize string
    std::string name = "yossi goldberg";
    std::strcpy(buffer + index, name.c_str());
    index += name.length() + 1;  // Include null terminator

    // Update buffer_len to actual used size
    buffer_len = index;

    // Create CSV string
    std::stringstream my_stream;
    my_stream << timestamp << ","
              << src << ","
              << dest << ","
              << buffer_len << ","
              << std::string(buffer, buffer_len);  // Convert buffer to string

    // Split CSV string
    std::vector<std::string> split_pieces;
    std::string piece;
    while (std::getline(my_stream, piece, ',')) {
        split_pieces.push_back(piece);
    }

    // Print first 4 pieces
    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
        std::cout << split_pieces[i] << std::endl;
    }

    // Deserialize buffer
    if (split_pieces.size() > 4) {
        const std::string& buffer_str = split_pieces[4];
        const char* exit_buffer = buffer_str.c_str();
        int exit_index = 0;

        // Extract int (age)
        int exit_age;
        std::memcpy(&exit_age, exit_buffer + exit_index, sizeof(exit_age));
        exit_index += sizeof(exit_age);

        // Extract double (high)
        double exit_high;
        std::memcpy(&exit_high, exit_buffer + exit_index, sizeof(exit_high));
        exit_index += sizeof(exit_high);

        // Extract string (name)
        std::string exit_name(exit_buffer + exit_index);

        // Print extracted values
        std::cout << "Extracted age: " << exit_age << std::endl;
        std::cout << "Extracted high: " << exit_high << std::endl;
        std::cout << "Extracted name: " << exit_name << std::endl;
    }
    QString converted_data = QString::fromStdString(my_stream.str());
    GlobalState::getInstance().new_test_buffer(converted_data);
}