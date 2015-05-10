#!/usr/bin/env python
#$Id$
#Copyright (c) 2013-2015 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS System libSystem
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Lesser General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Lesser General Public License for more details.
#
#You should have received a copy of the GNU Lesser General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



import _libSystem


#Config
class Config:
    def __init__(self):
        self.config = _libSystem.config_new()

    def get(self, section, variable):
        return _libSystem.config_get(self.config, section, variable)

    def set(self, section, variable, value):
        return _libSystem.config_set(self.config, section, variable, value)

    def load(self, filename):
        return _libSystem.config_load(self.config, filename)

    def reset(self):
        return _libSystem.config_reset(self.config)

    def save(self, filename):
        return _libSystem.config_save(self.config, filename)


#Event
class Event:
    def __init__(self):
        self.event = _libSystem.event_new()

    def loop(self):
        return _libSystem.event_loop(self.event)


#Mutator
class Mutator:
    def __init__(self):
        self.mutator = _libSystem.mutator_new()

    def get(self, key):
        return _libSystem.mutator_get(self.mutator, key)


#Plugin
class Plugin:
    def __init__(self):
        self.plugin = _libSystem.plugin_new()

    def lookup(self, symbol):
        return _libSystem.plugin_lookup(self.plugin, symbol)
