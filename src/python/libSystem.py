#!/usr/bin/env python2.7
#$Id$
#Copyright (c) 2013 Pierre Pronchery <khorben@defora.org>
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
		return _libSystem.config_set(self.config, section, variable,
				value)

	def load(self, filename):
		return _libSystem.config_load(self.config, filename)

	def save(self, filename):
		return _libSystem.config_save(self.config, filename)


#Event
class Event:
	def __init__(self):
		self.event = _libSystem.event_new()

	def loop(self):
		return _libSystem.event_loop(self.event)
