/*
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef LMBASE_GLOBAL_H
#define LMBASE_GLOBAL_H

#include <QtGlobal>

#if defined(LMBASE_LIBRARY)
#  define LMBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LMBASESHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // UBUNTU_GLOBAL_H

