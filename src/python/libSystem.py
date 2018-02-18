#!/usr/bin/env python
#$Id$
#Copyright (c) 2013-2018 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS System libSystem
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are
#met:
#
#1. Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
#2. Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
#TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



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
