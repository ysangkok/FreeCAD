/***************************************************************************
 *   Copyright (c) 2007 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
# include <QBuffer>
# include <QMessageBox>
# include <QTcpSocket>
#endif

#include <sstream>
//#include <CXX/Objects.hxx>
#include <zipios++/zipfile.h>
//#include <Base/Interpreter.h>
#include <Base/Stream.h>
#include <App/Application.h>

#include "MainWindow.h"
#include "BitmapFactory.h"
#include "OnlineDocumentation.h"

using namespace Gui;

// the favicon
static const unsigned int navicon_data_len = 318;
static const unsigned char navicon_data[] = {
    0x00,0x00,0x01,0x00,0x01,0x00,0x10,0x10,0x10,0x00,0x01,0x00,0x04,0x00,
    0x28,0x01,0x00,0x00,0x16,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x10,0x00,
    0x00,0x00,0x20,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
    0x84,0x82,0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x00,
    0x00,0x00,0x00,0x00,0x01,0x10,0x01,0x10,0x00,0x00,0x00,0x00,0x11,0x00,
    0x00,0x10,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x10,
    0x00,0x00,0x00,0x00,0x01,0x10,0x01,0x10,0x00,0x20,0x00,0x00,0x00,0x11,
    0x11,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,
    0x00,0x00,0x00,0x00,0x02,0x22,0x22,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0xff,0xff,0x00,0x00,0xfc,0x3f,0x00,0x00,0xf9,0x9f,0x00,0x00,
    0x93,0xdf,0x00,0x00,0x93,0xff,0x00,0x00,0x93,0xff,0x00,0x00,0x93,0xff,
    0x00,0x00,0x93,0xfd,0x00,0x00,0x81,0xd8,0x00,0x00,0x99,0x9d,0x00,0x00,
    0x9c,0x3d,0x00,0x00,0x9f,0xfd,0x00,0x00,0x80,0xfd,0x00,0x00,0xff,0x7d,
    0x00,0x00,0xfe,0x01,0x00,0x00,0xff,0x7f,0x00,0x00};

PythonOnlineHelp::PythonOnlineHelp()
{
}

PythonOnlineHelp::~PythonOnlineHelp()
{
}

QByteArray PythonOnlineHelp::loadResource(const QString& filename) const
{
    QString fn = filename;
    fn = filename.mid(1);
    QByteArray res;


    return res;
}

QByteArray PythonOnlineHelp::fileNotFound() const
{
    QString contentType = QString::fromLatin1(
        "text/html\r\n"
        "\r\n"
        "<html><head><title>Error</title></head>"
        "<body bgcolor=\"#f0f0f8\">"
        "<table width=\"100%\" cellspacing=0 cellpadding=2 border=0 summary=\"heading\">"
        "<tr bgcolor=\"#7799ee\">"
        "<td valign=bottom>&nbsp;<br>"
        "<font color=\"#ffffff\" face=\"helvetica, arial\">&nbsp;<br><big><big><strong>FreeCAD Documentation</strong></big></big></font></td>"
        "<td align=right valign=bottom>"
        "<font color=\"#ffffff\" face=\"helvetica, arial\">&nbsp;</font></td></tr></table>"
        "<p><p>"
        "<h1>404 - File not found</h1>"
        "<div><p><strong>The requested URL was not found on this server."
        "</strong></p>"
        "</div></body>"
        "</html>"
        "\r\n"
    );

    QString header = QString::fromLatin1("content-type: %1\r\n").arg(contentType);

    QString http(QLatin1String("HTTP/1.1 %1 %2\r\n%3\r\n"));
    QString httpResponseHeader = http.arg(404).arg(QLatin1String("File not found")).arg(header);

    QByteArray res;
    res.append(httpResponseHeader);
    return res;
}

HttpServer::HttpServer(QObject* parent)
  : QTcpServer(parent), disabled(false)
{
}

#if QT_VERSION >=0x050000
void HttpServer::incomingConnection(qintptr socket)
#else
void HttpServer::incomingConnection(int socket)
#endif
{
    if (disabled)
        return;

    // When a new client connects the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void HttpServer::pause()
{
    disabled = true;
}

void HttpServer::resume()
{
    disabled = false;
}

void HttpServer::readClient()
{
    if (disabled)
        return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a GET request and  sends back the
    // corresponding HTML document from the ZIP file.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QString httpRequestHeader = QString::fromLatin1(socket->readLine());
        QStringList lst = httpRequestHeader.simplified().split(QLatin1String(" "));
        QString method;
        QString path;
        if (lst.count() > 0) {
            QString m = lst[0];
            if (lst.count() > 1) {
                QString p = lst[1];
                if (lst.count() > 2) {
                    QString v = lst[2];
                    if (v.length() >= 8 && v.left(5) == QLatin1String("HTTP/") &&
                        v[5].isDigit() && v[6] == QLatin1Char('.') && v[7].isDigit()) {
                        method = m;
                        path = p;
                    }
                }
            }
        }

        if (method == QLatin1String("GET")) {
            socket->write(help.loadResource(path));
            socket->close();
            if (socket->state() == QTcpSocket::UnconnectedState) {
                //mark the socket for deletion but do not destroy immediately
                socket->deleteLater();
            }
        }
    }
}

void HttpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

// --------------------------------------------------------------------

/* TRANSLATOR Gui::StdCmdPythonHelp */

StdCmdPythonHelp::StdCmdPythonHelp()
  : Command("Std_PythonHelp"), server(0)
{
    sGroup        = QT_TR_NOOP("Tools");
    sMenuText     = QT_TR_NOOP("Automatic python modules documentation");
    sToolTipText  = QT_TR_NOOP("Opens a browser to show the Python modules documentation");
    sWhatsThis    = QT_TR_NOOP("Opens a browser to show the Python modules documentation");
    sStatusTip    = QT_TR_NOOP("Opens a browser to show the Python modules documentation");
    sPixmap       = "applications-python";
}

StdCmdPythonHelp::~StdCmdPythonHelp()
{
    if (server) {
        server->close();
        delete server;
    }
}

void StdCmdPythonHelp::activated(int iMsg)
{
}

bool Gui::OpenURLInBrowser(const char * URL)
{
  
    return true;
}


#include "moc_OnlineDocumentation.cpp"

