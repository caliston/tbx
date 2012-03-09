/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2012 Alan Buckley   All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *//*
 * questionwindow.cc
 *
 *  Created on: 8 Mar 2012
 *      Author: alanb
 */

#include "questionwindow.h"
#include "actionbutton.h"

namespace tbx {


/**
 * Show a question in a window centred in the screen.
 *
 * This shows the window and returns immediately. The commands given
 * are run when the use clicks on the appropriate button.
 *
 * To use this function a window template called "Question" must be in your
 * resources. A suitable window is provided in TbxRes in the !TBX directory.
 * For full details of this window see the QuestionWindow class.
 *
 * @param question The text of the question. This can be multi-line and the window
 *  will be resized to fit it.
 * @param title The title of the message box. Use "" to keep
 * the title from the Question resource.
 * @param yes_command command to run if yes is chosen or 0 for none.
 * @param no_command command to run if no is chosen or 0 (the default) for none
 */
void show_question(const std::string &question, const std::string &title, tbx::Command *yes_command, tbx::Command *no_command /*= 0*/)
{
   QuestionWindow *qw = new QuestionWindow(question);
   if (!title.empty()) qw->title(title);
   if (yes_command) qw->add_yes_command(yes_command);
   if (no_command) qw->add_no_command(no_command);
   qw->delete_on_hide();
   qw->show();
}

/**
 * Show a question in a window centred in the screen.
 *
 * This shows the window and returns immediately. The commands given
 * are run when the use clicks on the appropriate button.
 *
 * Selecting the "Yes" or "No" button or clicking outside the window close
 * the window. A command is not run if the window is closed by clicking
 * outsied of it.
 *
 * To use this function a window template called "Question" must be in your
 * resources. A suitable window is provided in TbxRes in the !TBX directory.
 * For full details of this window see the QuestionWindow class.
 *
 * @param question The text of the question. This can be multi-line and the window
 *  will be resized to fit it.
 * @param title The title of the message box. Use "" to keep
 * the title from the Question resource.
 * @param yes_command command to run if yes is chosen or 0 for none.
 * @param no_command command to run if no is chosen or 0 (the default) for none
 */
void show_question_as_menu(const std::string &question, const std::string &title, tbx::Command *yes_command, tbx::Command *no_command /*= 0*/)
{
   QuestionWindow *qw = new QuestionWindow(question);
   if (!title.empty()) qw->title(title);
   if (yes_command) qw->add_yes_command(yes_command);
   if (no_command) qw->add_no_command(no_command);
   qw->delete_on_hide();
   qw->show_as_menu();
}

/**
 * Construct a question window with the give question
 *
 * @param question The question for the window
 */
QuestionWindow::QuestionWindow(const std::string &question)
   : TextDisplayWindow("Question", -1,-1, 1,2, question)
{
}

QuestionWindow::~QuestionWindow()
{
}

/**
 * Add a command to be run if the yes button is selected
 *
 * @param yes_command command to run if "Yes" is selected.
 */
void QuestionWindow::add_yes_command(tbx::Command *yes_command)
{
	tbx::ActionButton yes_button = _window.gadget(1);
	yes_button.add_selected_command(yes_command);
}

/**
 * Add a command to be run if the no button is selected
 *
 * @param no_command command to run if "No" is selected.
 */
void QuestionWindow::add_no_command(tbx::Command *no_command)
{
	tbx::ActionButton no_button = _window.gadget(2);
	no_button.add_selected_command(no_command);
}

} /* namespace tbx */
