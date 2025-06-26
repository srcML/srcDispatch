##
#  CMakeLists.txt
#
#  Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
#
#  This file is part of the srcDiffDispatch.
#
#  The srcDiffDispatch is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  The srcDiffDispatch is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the srcDiffDispatch; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

file(GLOB TESTS *.cpp)

foreach(TEST IN ITEMS ${TESTS})
	add_unit_test(${TEST})
endforeach()
