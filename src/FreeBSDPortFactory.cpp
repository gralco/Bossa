///////////////////////////////////////////////////////////////////////////////
// BOSSA
//
// Copyright (C) 2011-2012 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#include "FreeBSDPortFactory.h"
#include "PosixSerialPort.h"

#include <string.h>
#include <stdio.h>

#include <string>

FreeBSDPortFactory::FreeBSDPortFactory()
{
    _dir = opendir("/dev");
}

FreeBSDPortFactory::~FreeBSDPortFactory()
{
    if (_dir)
        closedir(_dir);
}

SerialPort::Ptr
FreeBSDPortFactory::create(const std::string& name)
{
    bool isUsb = false;

    if (name.find("ttyU") != std::string::npos)
        isUsb = true;

    return create(name, isUsb);
}

SerialPort::Ptr
FreeBSDPortFactory::create(const std::string& name, bool isUsb)
{
    return SerialPort::Ptr(new PosixSerialPort(name, isUsb));
}

std::string
FreeBSDPortFactory::begin()
{
    if (!_dir)
        return end();

    rewinddir(_dir);

    return next();
}

std::string
FreeBSDPortFactory::next()
{
    struct dirent* entry;

    if (!_dir)
        return end();

    while ((entry = readdir(_dir)))
    {
        if (strncmp("ttyU", entry->d_name, sizeof("ttyU") - 1) == 0)
            return std::string(entry->d_name);
        else if (strncmp("ttyS", entry->d_name, sizeof("ttyS") - 1) == 0)
            return std::string(entry->d_name);
    }

    return end();
}

std::string
FreeBSDPortFactory::end()
{
    return std::string();
}
