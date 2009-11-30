/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "actionbutton.h"
#include "pollinfo.h"
#include "buttonselectedlistener.h"
#include "command.h"
#include "commandrouter.h"

#include "kernel.h"

namespace tbx {

static void button_selected_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	ButtonSelectedEvent event(ActionButton(id_block.self_component()), data.word[3]);
	static_cast<ButtonSelectedListener *>(listener)->button_selected(event);
}

static void button_select_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	if ((data.word[3] & 1) == 0)
		static_cast<Command *>(listener)->execute();
}

static void button_adjust_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	if ((data.word[3] & 1) != 0)
		static_cast<Command *>(listener)->execute();
}

/**
 * Add listener for button selected with the select or adjust mouse buttons
 * or the keyboard (return or escape)
 */
void ActionButton::add_selected_listener(ButtonSelectedListener *listener)
{
	add_listener(0x82881, listener, button_selected_router);
}

/**
 * Remove listener for button selected with the select or adjust mouse buttons
 * or the keyboard (return or escape)
 */
void ActionButton::remove_selected_listener(ButtonSelectedListener *listener)
{
	remove_listener(0x82881, listener);
}

/**
 * Add command to be run if the button is selected with the select or adjust
 * mouse buttons or the keyboard (return or escape)
 */
void ActionButton::add_selected_command(Command *command)
{
	add_listener(0x82881, command, command_router);
}

/**
 * Remove command to be run if the button is selected with the select or adjust
 * mouse buttons or the keyboard (return or escape)
 */
void ActionButton::remove_selected_command(Command *command)
{
	remove_listener(0x82881, command);
}

/**
 * Add command to be run if the button is selected with the select mouse button
 * or the keyboard (return or escape)
 */

void ActionButton::add_select_command(Command *command)
{
	add_listener(0x82881, command, button_select_router);
}

/**
 * Remove command to be run if the button is selected with the select mouse button
 * or the keyboard (return or escape)
 */
void ActionButton::remove_select_command(Command *command)
{
	remove_listener(0x82881, command);
}

/**
 * Add command to be run if the button is selected with the adjust mouse button
 * only
 */
void ActionButton::add_adjust_command(Command *command)
{
	add_listener(0x82881, command, button_adjust_router);
}

/**
 * Remove command to be run if the button is selected with the adjust mouse button
 * only
 */
void ActionButton::remove_adjust_command(Command *command)
{
	remove_listener(0x82881, command);
}

}
