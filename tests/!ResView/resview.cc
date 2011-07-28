/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2010 Alan Buckley   All Rights Reserved.
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
 */

#include "tbx/res/reseditor.h"

#include "tbx/res/rescolourdbox.h"
#include "tbx/res/rescolourmenu.h"
#include "tbx/res/resdcs.h"
#include "tbx/res/resfileinfo.h"
#include "tbx/res/resfontdbox.h"
#include "tbx/res/resfontmenu.h"
#include "tbx/res/resiconbar.h"
#include "tbx/res/resmenu.h"
#include "tbx/res/resprintdbox.h"
#include "tbx/res/resproginfo.h"
#include "tbx/res/resquit.h"
#include "tbx/res/ressaveas.h"
#include "tbx/res/reswindow.h"

#include "tbx/res/resactionbutton.h"
#include "tbx/res/resadjuster.h"
#include "tbx/res/resbutton.h"
#include "tbx/res/resdisplayfield.h"
#include "tbx/res/resdraggable.h"
#include "tbx/res/reslabel.h"
#include "tbx/res/reslabelledbox.h"
#include "tbx/res/resnumberrange.h"
#include "tbx/res/resoptionbutton.h"
#include "tbx/res/respopup.h"
#include "tbx/res/resradiobutton.h"
#include "tbx/res/resscrolllist.h"
#include "tbx/res/resslider.h"
#include "tbx/res/resstringset.h"
#include "tbx/res/restextarea.h"
#include "tbx/res/restoolaction.h"
#include "tbx/res/reswritablefield.h"

using namespace tbx::res;
using namespace std;

void dump_object(std::ostream &os, const ResObject &obj);
void dump_unknown(std::ostream &os, const ResObject &obj);

void dump_colourdbox(std::ostream &os, const ResObject &obj);
void dump_dcs(std::ostream &os, const ResObject &obj);
void dump_iconbar(std::ostream &os, const ResObject &obj);
void dump_menu(std::ostream &os, const ResObject &obj);
void dump_menuitem(std::ostream &os, const ResMenuItem &item);
void dump_proginfo(std::ostream &os, const ResObject &obj);
void dump_saveas(std::ostream &os, const ResObject &obj);
void dump_window(std::ostream &os, const ResObject &obj);

void dump_colourdbox(std::ostream &os, const ResObject &obj);
void dump_colourmenu(std::ostream &os, const ResObject &obj);
void dump_fileinfo(std::ostream &os, const ResObject &obj);
void dump_fontdbox(std::ostream &os, const ResObject &obj);
void dump_fontmenu(std::ostream &os, const ResObject &obj);
void dump_printdbox(std::ostream &os, const ResObject &obj);
void dump_quit(std::ostream &os, const ResObject &obj);

void dump_gadget(std::ostream &os, const ResGadget &gadget);
void dump_shortcut(std::ostream &os, const ResShortcut &ks);

// Gadget dumping
void dump_unknown_gadget(std::ostream &os, const ResGadget &gadget);
void dump_actionbutton(std::ostream &os, const ResGadget &gadget);
void dump_adjuster(std::ostream &os, const ResGadget &gadget);
void dump_button(std::ostream &os, const ResGadget &gadget);
void dump_displayfield(std::ostream &os, const ResGadget &gadget);
void dump_draggable(std::ostream &os, const ResGadget &gadget);
void dump_label(std::ostream &os, const ResGadget &gadget);
void dump_labelledbox(std::ostream &os, const ResGadget &gadget);
void dump_numberrange(std::ostream &os, const ResGadget &gadget);
void dump_optionbutton(std::ostream &os, const ResGadget &gadget);
void dump_popup(std::ostream &os, const ResGadget &gadget);
void dump_radiobutton(std::ostream &os, const ResGadget &gadget);
void dump_scrolllist(std::ostream &os, const ResGadget &gadget);
void dump_slider(std::ostream &os, const ResGadget &gadget);
void dump_stringset(std::ostream &os, const ResGadget &gadget);
void dump_textarea(std::ostream &os, const ResGadget &gadget);
void dump_toolaction(std::ostream &os, const ResGadget &gadget);
void dump_writablefield(std::ostream &os, const ResGadget &gadget);

const char *MARGIN = "   ";

struct OC
{
	int class_id;
	const char *name;
	void (*dump_func)(std::ostream &os, const ResObject &obj);
} ObjectClasses[] =
{
	{0x82900, "Iconbar", &dump_iconbar},
	{0x828c0, "Menu", &dump_menu},
	{0x82b40, "ProgInfo", &dump_proginfo},
	{0x82880, "Window", &dump_window},
	{0x82bc0, "SaveAs", &dump_saveas},
	{0x82a80, "Discard/Cancel/Save", &dump_dcs},
	{0x829C0, "ColourDbox", &dump_colourdbox},
	{0x82980, "ColourMenu", &dump_colourmenu},
	{0x82ac0, "FileInfo", &dump_fileinfo},
	{0x82a00, "FontDbox", &dump_fontdbox},
	{0x82a40, "FontMenu", &dump_fontmenu},
	{0x82b00, "PrintDbox", &dump_printdbox},
	{0x82a90, "Quit", &dump_quit},

	// Last dumps information on class we haven't details on
	{0, "Unknown", &dump_unknown}
};

struct GC
{
	int type_id;
	const char *name;
	void (*dump_func)(std::ostream &os, const ResGadget &obj);
} GadgetClasses[] =
{
	{128, "Action Button", &dump_actionbutton},
	{192, "Option Button", &dump_optionbutton},
	{256, "Labelled Box", &dump_labelledbox},
	{320, "Label", &dump_label},
	{384, "Radio Button", &dump_radiobutton},
	{448, "Display Field", &dump_displayfield},
	{512, "Writable Field", &dump_writablefield},
	{576, "Slider", &dump_slider},
	{640, "Draggable", &dump_draggable},
	{704, "PopUp Menu", &dump_popup},
	{768, "Adjuster Arrow", &dump_adjuster},
	{832, "Number Range", &dump_numberrange},
	{896, "String Set", &dump_stringset},
	{960, "Button", &dump_button},
	{0x4014, "Tool Action", &dump_toolaction},
	{0x4018, "Text area", &dump_textarea},
	{0x401A, "Scroll list", &dump_scrolllist},

	// Last dumps information on class we haven't details on
	{0, "Unknown", &dump_unknown_gadget}
};


extern void do_tests();

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Usage: resview <filename>" << endl;
		return -1;
	}
	char *test_file = argv[1];

	ResEditor editor;
	if (!editor.load(test_file))
	{
		cerr << "Unable to load " << test_file << endl;
		return -1;
	}
	cout << test_file << " loaded" << endl;

    for (ResEditor::const_iterator i = editor.begin();
              i != editor.end(); ++i)
   {
       // use a set
        const ResObject &obj = *i;

		cout << "Found object " << obj.name() << endl;
		dump_object(cout, obj);
    }

	return 0;
}

void dump_object(std::ostream &os, const ResObject &obj)
{
	os << MARGIN << "Class ID: " << obj.class_id();

	int cidx = 0;
	while (ObjectClasses[cidx].class_id != obj.class_id()
		&& ObjectClasses[cidx].class_id != 0) cidx++;
	os << " " << ObjectClasses[cidx].name << endl;
	os << MARGIN << "Version : " << obj.object_version() << endl;
	os << MARGIN << "Object flags: " << obj.object_flags() << endl;
	if (obj.create_on_load()) os << MARGIN << " create on load" << endl;
	if (obj.show_on_create()) os << MARGIN << " show on create" << endl;
	if (obj.shared()) os << MARGIN << " shared" << endl;
	if (obj.ancestor()) os << MARGIN << " ancestor" << endl;

	(*ObjectClasses[cidx].dump_func)(os, obj);
}

void dump_unknown(std::ostream &os, const ResObject &obj)
{
}

inline const char *string_or_null(const char *str)
{
	return (str) ? str : "(null)";
}

void dump_iconbar(std::ostream &os, const ResObject &obj)
{
	ResIconbar ib(obj);
	os << MARGIN << "Flags: " << hex << ib.flags() << dec << endl;
	if (ib.generate_select_about_to_be_shown()) os << MARGIN << " Generate select about to be shown" << endl;
	if (ib.generate_adjust_about_to_be_shown()) os << MARGIN << " Generate adjust about to be shown" << endl;
	if (ib.select_show_transient()) os << MARGIN << " Select show is transient" << endl;
	if (ib.adjust_show_transient()) os << MARGIN << " Adjust show is transient" << endl;
	if (ib.generate_clicked_on_select()) os << MARGIN << " Generate clicked on select" << endl;
	if (ib.generate_clicked_on_adjust()) os << MARGIN << " Generate clicked on adjust" << endl;
	os << MARGIN << "Position: " << ib.position() << endl;
	os << MARGIN << "Priority: " << ib.priority() << endl;
	os << MARGIN << "Sprite Name: " <<  string_or_null(ib.sprite_name()) << endl;
	os << MARGIN << "Max length of sprite name: " << ib.max_sprite_name() << endl;
	os << MARGIN << "Text: " << string_or_null(ib.text()) << endl;
	os << MARGIN << "Maximum length of text: " << ib.max_text() << endl;
	os << MARGIN << "Menu: " << string_or_null(ib.menu()) << endl;
	os << MARGIN << "Select event: " << ib.select_event() << endl;
	os << MARGIN << "Adjust event: " << ib.adjust_event() << endl;
	os << MARGIN << "Select show: " << string_or_null(ib.select_show()) << endl;
	os << MARGIN << "Adjust show: " << string_or_null(ib.adjust_show()) << endl;
	os << MARGIN << "Help message: " << string_or_null(ib.help_message()) << endl;
	os << MARGIN << "Max help message length: " << ib.max_help_message() << endl;

}


void dump_menu(std::ostream &os, const ResObject &obj)
{
	ResMenu menu(obj);

	os << MARGIN << "Flags: " << hex << menu.flags() << dec << endl;
	if (menu.generate_about_to_be_shown()) os << MARGIN << " Generate event when shown" << endl;
	if (menu.generate_has_been_hidden()) os << MARGIN << " Generate event when hidden" << endl;
	os << MARGIN << "Title: " << string_or_null(menu.title()) << endl;
	os << MARGIN << "Maximum length of title: " << menu.max_title() << endl;
	os << MARGIN << "Help message: " << string_or_null(menu.help_message()) << endl;
	os << MARGIN << "Max help message length: " << menu.max_help_message() << endl;
	os << MARGIN << "About to be shown event: " << menu.about_to_be_shown_event() << endl;
	os << MARGIN << "Has been hidden event: " << menu.has_been_hidden_event() << endl;
	os << MARGIN << "Item count: " << menu.item_count() << endl;

	for (ResMenu::const_iterator i = menu.cbegin(); i != menu.cend(); ++i)
	{
		dump_menuitem(os, *i);
	}
}

void dump_menuitem(std::ostream &os, const ResMenuItem &item)
{
	os << MARGIN << " Menu item" << endl;
	os << MARGIN << MARGIN << "Flags: " << hex << item.flags() << dec << endl;
	if (item.ticked()) os << MARGIN << MARGIN << " Ticked" << endl;
	if (item.dotted_line()) os << MARGIN << MARGIN << " Dotted line" << endl;
	if (item.faded()) os << MARGIN << MARGIN << " Faded" << endl;
	if (item.sprite()) os << MARGIN << MARGIN << " Is sprite" << endl;
	if (item.has_submenu()) os << MARGIN << MARGIN << " Has submenu" << endl;
	if (item.generate_submenu_event()) os << MARGIN << MARGIN << " Generate submenu event" << endl;
	if (item.show_transient()) os << MARGIN << MARGIN << " Show transient" << endl;
	os << MARGIN << MARGIN << "Component id: " << item.component_id() << endl;
	os << MARGIN << MARGIN << "Text: " << string_or_null(item.text()) << endl;
	os << MARGIN << MARGIN << "Max text: " << item.max_text() << endl;
	os << MARGIN << MARGIN << "Click show: " << string_or_null(item.click_show()) << endl;
	os << MARGIN << MARGIN << "Submenu show: " << string_or_null(item.submenu_show()) << endl;
	os << MARGIN << MARGIN << "Submenu event: " << item.submenu_event() << endl;
	os << MARGIN << MARGIN << "Click event: " << item.click_event() << endl;
	os << MARGIN << MARGIN << "Help message: " << string_or_null(item.help_message()) << endl;
    os << MARGIN << MARGIN << "Max help message: " << item.max_help_message() << endl;

}

void dump_proginfo(std::ostream &os, const ResObject &obj)
{
	ResProgInfo pi(obj);

	os << MARGIN << "Flags: " << hex << pi.flags() << dec << endl;
	if (pi.generate_about_to_be_shown()) os << MARGIN << " Generate event when shown" << endl;
	if (pi.generate_dialogue_completed()) os << MARGIN << " Generate event when dialogue completed" << endl;
	if (pi.include_licence_type()) os << MARGIN << " Shows licence type on dialogue" << endl;
	os << MARGIN << "Title: " << string_or_null(pi.title()) << endl;
	os << MARGIN << "Maximum length of title: " << pi.max_title() << endl;
	os << MARGIN << "Purpose: " << string_or_null(pi.purpose()) << endl;
	os << MARGIN << "Author: " << string_or_null(pi.author()) << endl;
	os << MARGIN << "License type: " << pi.licence_type() << endl;
	os << MARGIN << "Version: " << string_or_null(pi.version()) << endl;
	os << MARGIN << "Window: " << string_or_null(pi.window()) << endl;
}

void dump_saveas(std::ostream &os, const ResObject &obj)
{
	ResSaveAs g(obj);

	os << MARGIN << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << MARGIN << " generate_dialogue_completed" << endl;
	if (g.no_selection())	os << MARGIN << MARGIN << " no_selection" << endl;
	if (g.use_buffer())	os << MARGIN << MARGIN << " use_buffer" << endl;
	if (g.ram_transfer())	os << MARGIN << MARGIN << " ram_transfer" << endl;
	os << MARGIN << MARGIN << "Filename: " << string_or_null(g.file_name()) << endl;
	os << MARGIN << MARGIN << "Filetype: " << hex << g.file_type() << dec << endl;
	os << MARGIN << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_dcs(std::ostream &os, const ResObject &obj)
{
	ResDCS g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << "Alternative Message: " << string_or_null(g.alternative_message()) << endl;
	os << MARGIN << "Max_message: " << g.max_alternative_message() << endl;
	os << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_colourdbox(std::ostream &os, const ResObject &obj)
{
	ResColourDbox g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	if (g.include_none())	os << MARGIN << " include_none" << endl;
	if (g.select_none())	os << MARGIN << " select_none" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << "Colour: " << g.colour() << endl;
}

void dump_colourmenu(std::ostream &os, const ResObject &obj)
{
	ResColourMenu g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_has_been_hidden())	os << MARGIN << " generate_has_been_hidden" << endl;
	if (g.include_none())	os << MARGIN << " include_none" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max-title: " << g.max_title() << endl;
	os << MARGIN << "Colour: " << g.colour() << endl;
}

void dump_fileinfo(std::ostream &os, const ResObject &obj)
{
	ResFileInfo g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max title: " << g.max_title() << endl;
	os << MARGIN << "Modified: " << g.modified() << endl;
	os << MARGIN << "Filetype: " << g.file_type() << endl;
	os << MARGIN << "Filename: " << string_or_null(g.file_name()) << endl;
	os << MARGIN << "Filesize: " << g.filesize() << endl;
//TODO:	os << MARGIN << "Date: " << g.date() << endl;
	os << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_fontdbox(std::ostream &os, const ResObject &obj)
{
	ResFontDbox g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	if (g.include_system_font())	os << MARGIN << " include_system_font" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << "Initial_font: " << string_or_null(g.initial_font()) << endl;
	os << MARGIN << "Initial_height: " << g.initial_height() << endl;
	os << MARGIN << "Initial_aspect: " << g.initial_aspect() << endl;
	os << MARGIN << "Try_string: " << string_or_null(g.try_string()) << endl;
	os << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_printdbox(std::ostream &os, const ResObject &obj)
{
	ResPrintDbox g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	if (g.generate_setup_about_to_be_shown())	os << MARGIN << " generate_setup_about_to_be_shown" << endl;
	if (g.has_page_range())	os << MARGIN << " has_page_range" << endl;
	if (g.has_copies())	os << MARGIN << " has_copies" << endl;
	if (g.has_scale())	os << MARGIN << " has_scale" << endl;
	if (g.has_orientation())	os << MARGIN << " has_orientation" << endl;
	if (g.has_save())	os << MARGIN << " has_save" << endl;
	if (g.has_setup())	os << MARGIN << " has_setup" << endl;
	if (g.has_draft())	os << MARGIN << " has_draft" << endl;
	if (g.has_from_to())	os << MARGIN << " has_from_to" << endl;
	if (g.sideways_selected())	os << MARGIN << " sideways_selected" << endl;
	if (g.draft_selected())	os << MARGIN << " draft_selected" << endl;
	os << MARGIN << "From: " << g.from() << endl;
	os << MARGIN << "To: " << g.to() << endl;
	os << MARGIN << "Copies: " << g.copies() << endl;
	os << MARGIN << "Scale: " << g.scale() << endl;
	os << MARGIN << "Further_options: " << string_or_null(g.further_options()) << endl;
	os << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_quit(std::ostream &os, const ResObject &obj)
{
	ResQuit g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << " generate_dialogue_completed" << endl;
	os << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << "Message: " << string_or_null(g.alternative_message()) << endl;
	os << MARGIN << "Max_message: " << g.max_alternative_message() << endl;
	os << MARGIN << "Window: " << string_or_null(g.window()) << endl;
}

void dump_fontmenu(std::ostream &os, const ResObject &obj)
{
	ResFontMenu g(obj);

	os << MARGIN << "Flags: " << g.flags() << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_has_been_hidden())	os << MARGIN << " generate_has_been_hidden" << endl;
	if (g.include_system_font())	os << MARGIN << " include_system_font" << endl;
	os << MARGIN << "Ticked_font: " << string_or_null(g.ticked_font()) << endl;
}

void dump_window(std::ostream &os, const ResObject &obj)
{
	ResWindow rw(obj);
	os << MARGIN << "Flags: " << hex << rw.flags() << dec << endl;
	if (rw.generate_about_to_be_shown()) os << MARGIN << " Generate event when shown" << endl;
	if (rw.auto_show()) os << MARGIN << " Show window on creation" << endl;
	if (rw.auto_close()) os << MARGIN << " Automatically close window" << endl;
	if (rw.generate_has_been_hidden()) os << MARGIN << " Generate event when hiden" << endl;
	os << MARGIN << "Help message: " << string_or_null(rw.help_message()) << endl;
	os << MARGIN << "Max help: " << rw.max_help_message() << endl;
	os << MARGIN << "Pointer shape: " << string_or_null(rw.pointer_shape()) << endl;
	os << MARGIN << "Max pointer shape: " << rw.max_pointer_shape() << endl;
	os << MARGIN << "Pointer X hot: " << rw.pointer_x_hot() << endl;
	os << MARGIN << "Pointer Y hot: " << rw.pointer_y_hot() << endl;
	os << MARGIN << "Menu: " << string_or_null(rw.menu()) << endl;
	os << MARGIN << "Num keyboard shortcuts: " << rw.num_shortcuts() << endl;
	os << MARGIN << "Num gadgets: " << rw.num_gadgets() << endl;
	os << MARGIN << "Default focus id: " << rw.default_focus() << endl;
	os << MARGIN << "Show event: " << rw.show_event() << endl;
	os << MARGIN << "Hide event: " << rw.hide_event() << endl;
	os << MARGIN << "internal bl toolbar: " << string_or_null(rw.internal_bl_toolbar()) << endl;
	os << MARGIN << "internal tl toolbar: " << string_or_null(rw.internal_tl_toolbar()) << endl;
	os << MARGIN << "external bl toolbar: " << string_or_null(rw.external_bl_toolbar()) << endl;
	os << MARGIN << "external tl toolbar: " << string_or_null(rw.external_tl_toolbar()) << endl;
	os << MARGIN << "visible_xmin: " << rw.visible_xmin() << endl;
	os << MARGIN << "visible_ymin: " << rw.visible_ymin() << endl;
	os << MARGIN << "visible_xmax: " << rw.visible_xmax() << endl;
	os << MARGIN << "visible_ymax: " << rw.visible_ymax() << endl;
	os << MARGIN << "scroll_x: " << rw.scroll_x() << endl;
	os << MARGIN << "scroll_y: " << rw.scroll_y() << endl;
	os << MARGIN << "behind: " << rw.behind() << endl;
	os << MARGIN << "window_flags: " << hex << rw.window_flags() << dec << endl;
	os << MARGIN << "title_fore: " << rw.title_fore() << endl;
	os << MARGIN << "title_back: " << rw.title_back() << endl;
	os << MARGIN << "work_fore: " << rw.work_fore() << endl;
	os << MARGIN << "work_back: " << rw.work_back() << endl;
	os << MARGIN << "scroll_outer: " << rw.scroll_outer() << endl;
	os << MARGIN << "scroll_inner: " << rw.scroll_inner() << endl;
	os << MARGIN << "title_inputfocus: " << rw.title_inputfocus() << endl;

	os << MARGIN << "work_xmin: " << rw.work_xmin() << endl;
	os << MARGIN << "work_ymin: " << rw.work_ymin() << endl;
	os << MARGIN << "work_xmax: " << rw.work_xmax() << endl;
	os << MARGIN << "work_ymax: " << rw.work_ymax() << endl;
	os << MARGIN << "title_flags: " << hex << rw.title_flags() << dec << endl;
	os << MARGIN << "button_type: " << hex << rw.button_type() << dec << endl;
	os << MARGIN << "sprite_area: " << hex << (int)rw.sprite_area() << dec << endl;

	os << MARGIN << "min_width: " << rw.min_width() << endl;
	os << MARGIN << "min_height: " << rw.min_height() << endl;

	os << MARGIN << "title_text: " << string_or_null(rw.title_text()) << endl;
	os << MARGIN << "title_validation: " << string_or_null(rw.title_validation()) << endl;
	os << MARGIN << "title_buflen: " << rw.title_buflen() << endl;

	for (ResWindow::const_gadget_iterator i = rw.gadget_cbegin(); i != rw.gadget_cend(); ++i)
	{
		dump_gadget(os, *i);
	}

	for (ResWindow::const_shortcut_iterator i = rw.shortcut_cbegin(); i != rw.shortcut_cend(); ++i)
	{
		dump_shortcut(os, *i);
	}

}

void dump_gadget(std::ostream &os, const ResGadget &gadget)
{
	int cidx = 0;
	while (GadgetClasses[cidx].type_id != gadget.type()
		&& GadgetClasses[cidx].type_id != 0) cidx++;

	os << MARGIN << " Gadget" << endl;
	os << MARGIN << MARGIN << "Flags: " << hex << gadget.flags() << dec << endl;
	if (gadget.back()) os << MARGIN << MARGIN << " At back" << endl;
	if (gadget.faded()) os << MARGIN << MARGIN << " Faded" << endl;
	os << MARGIN << MARGIN << "Type: " << gadget.type() << " " << GadgetClasses[cidx].name << endl;
	os << MARGIN << MARGIN << "Size: " << gadget.size() << " (stored " << gadget.stored_size() <<")" << endl;
	os << MARGIN << MARGIN << "xmin: " << gadget.xmin() << endl;
	os << MARGIN << MARGIN << "ymin: " << gadget.ymin() << endl;
	os << MARGIN << MARGIN << "xmax: " << gadget.xmax() << endl;
	os << MARGIN << MARGIN << "ymax: " << gadget.ymax() << endl;
	os << MARGIN << MARGIN << "Component Id: " << gadget.component_id() << endl;
	os << MARGIN << MARGIN << "Help message: " << string_or_null(gadget.help_message()) << endl;
	os << MARGIN << MARGIN << "Max help: " << gadget.max_help() << endl;

	(*GadgetClasses[cidx].dump_func)(os, gadget);

}

void dump_shortcut(std::ostream &os, const ResShortcut &ks)
{
	os << MARGIN << " Keyboard Shortcut" << endl;
	os << MARGIN << MARGIN << "Flags: " << hex << ks.flags() << dec << endl;
	if (ks.transient()) os << MARGIN << MARGIN << " transient" << endl;
	os << MARGIN << MARGIN << "Wimp Key code: " << ks.key_code() << endl;
	os << MARGIN << MARGIN << "Event: " << ks.event_id() << endl;
	os << MARGIN << MARGIN << "Show object: " << string_or_null(ks.show()) << endl;
}

void dump_actionbutton(std::ostream &os, const ResGadget &gadget)
{
	ResActionButton g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.is_default())	os << MARGIN << MARGIN << " default" << endl;
	if (g.cancel())	os << MARGIN << MARGIN << " cancel" << endl;
	if (g.local())	os << MARGIN << MARGIN << " local" << endl;
	if (g.click_show_transient())	os << MARGIN << MARGIN << " click_show_transient" << endl;
	os << MARGIN << MARGIN << "Text: " << g.text() << endl;
	os << MARGIN << MARGIN << "Max_text_len: " << g.max_text_len() << endl;
	os << MARGIN << MARGIN << "Click_show: " << string_or_null(g.click_show()) << endl;
	os << MARGIN << MARGIN << "Event: " << g.event() << endl;
}

void dump_adjuster(std::ostream &os, const ResGadget &gadget)
{
	ResAdjuster adj(gadget);
	os << MARGIN << MARGIN << "Adjuster has no fields" << endl;
}

void dump_button(std::ostream &os, const ResGadget &gadget)
{
	static char *ButtonTypes[] =
	{
		"IGNORE_CLICKS",
		"CONTINUOUS",
		"CLICK_AUTO_REPEAT",
		"CLICK_ONCE",
		"CLICK_SELECTS",
		"CLICK_SELECTS_DOUBLE",
		"CLICK_ONCE_DRAG",
		"CLICK_SELECTS_DRAG",
		"CLICK_SELECTS_DOUBLE_DRAG",
		"OVER_SELECTS",
		"CLICK_DOUBLE_DRAG",
		"CLICK_NOTIFY_AND_SELECTS_DRAG",
		"",
		"",
		"FOCUS_AND_DRAG",
		"WRITEABLE"
	};

	ResButton g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.use_task_sprite_area())	os << MARGIN << MARGIN << " use_task_sprite_area" << endl;
	if (g.return_menu_clicks())	os << MARGIN << MARGIN << " return_menu_clicks" << endl;
	os << MARGIN << MARGIN << "Button_flags: " << g.button_flags() << endl;
	if (g.has_text()) os << MARGIN << MARGIN << " has_text" << endl;
	if (g.is_sprite()) os << MARGIN << MARGIN << " is_sprite" << endl;
	if (g.has_border()) os << MARGIN << MARGIN << " has_border" << endl;
	if (g.hcentred()) os << MARGIN << MARGIN << " hcentred" << endl;
	if (g.vcentred()) os << MARGIN << MARGIN << " vcentred" << endl;
	if (g.filled()) os << MARGIN << MARGIN << " filled" << endl;
	if (g.anti_aliased_font()) os << MARGIN << MARGIN << " anti_aliased_font" << endl;
	if (g.redraw_needs_help()) os << MARGIN << MARGIN << " redraw_needs_help" << endl;
	if (g.text_right_justified()) os << MARGIN << MARGIN << " text_right_justified" << endl;
	if (g.adjust_does_not_cancel()) os << MARGIN << MARGIN << " adjust_does_nott_cancel" << endl;
	if (g.half_size_sprite()) os << MARGIN << MARGIN << " half_size_sprite" << endl;
	os << MARGIN << MARGIN << " Button type: " << ButtonTypes[g.button_type()] << endl;
	os << MARGIN << MARGIN << " ESG: "<< g.esg() << endl;

	if (g.anti_aliased_font())
	{
		os << MARGIN << MARGIN << " Font handle: " << g.font_handle() << endl;
	} else
	{
		os << MARGIN << MARGIN << " Foreground: " << g.foreground() << endl;
		os << MARGIN << MARGIN << " Background: " << g.background() << endl;
	}

	os << MARGIN << MARGIN << "Value: " << string_or_null(g.value()) << endl;
	os << MARGIN << MARGIN << "Max_value: " << g.max_value() << endl;
	os << MARGIN << MARGIN << "Validation: " << string_or_null(g.validation()) << endl;
	os << MARGIN << MARGIN << "Max_validation: " << g.max_validation() << endl;
}

void dump_label(std::ostream &os, const ResGadget &gadget)
{
	ResLabel g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.omit_bounding_box())	os << MARGIN << MARGIN << " omit_bounding_box" << endl;
	os << MARGIN << MARGIN << "Label: " << g.label() << endl;
}

void dump_displayfield(std::ostream &os, const ResGadget &gadget)
{
	ResDisplayField g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	os << MARGIN << MARGIN << " Justification : " << g.justification() << endl;
	os << MARGIN << MARGIN << "Text: " << string_or_null(g.text()) << endl;
	os << MARGIN << MARGIN << "Max_text_len: " << g.max_text_len() << endl;
}


void dump_unknown_gadget(std::ostream &os, const ResGadget &gadget)
{
	os << MARGIN << MARGIN << "TODO: This gadget type" << endl;
}

void dump_optionbutton(std::ostream &os, const ResGadget &gadget)
{
	ResOptionButton g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_state_changed())	os << MARGIN << MARGIN << " generate_state_changed" << endl;
	if (g.on())	os << MARGIN << MARGIN << " on" << endl;
	os << MARGIN << MARGIN << "Flags: " << g.flags() << endl;
	os << MARGIN << MARGIN << "Label: " << string_or_null(g.label()) << endl;
	os << MARGIN << MARGIN << "Max_label_len: " << g.max_label_len() << endl;
	os << MARGIN << MARGIN << "Event: " << g.event_id() << endl;
}

void dump_labelledbox(std::ostream &os, const ResGadget &gadget)
{
	ResLabelledBox g(gadget);

	os << MARGIN << MARGIN << "Label: " << string_or_null(g.label()) << endl;
}

void dump_radiobutton(std::ostream &os, const ResGadget &gadget)
{
	ResRadioButton g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_state_changed())	os << MARGIN << MARGIN << " generate_state_changed" << endl;
	if (g.on())	os << MARGIN << MARGIN << " on" << endl;
	os << MARGIN << MARGIN << "Group_number: " << g.group_number() << endl;
	os << MARGIN << MARGIN << "Label: " << string_or_null(g.label()) << endl;
	os << MARGIN << MARGIN << "Max_label_len: " << g.max_label_len() << endl;
	os << MARGIN << MARGIN << "Event: " << g.event() << endl;
}

void dump_writablefield(std::ostream &os, const ResGadget &gadget)
{
	ResWritableField g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_value_changed())	os << MARGIN << MARGIN << " generate_value_changed" << endl;
	os << MARGIN << MARGIN << " Justification : " << g.justification() << endl;
	if (g.do_not_display_text())	os << MARGIN << MARGIN << " do_not_display_text" << endl;
	os << MARGIN << MARGIN << "Text: " << string_or_null(g.text()) << endl;
	os << MARGIN << MARGIN << "Max_text_len: " << g.max_text_len() << endl;
	os << MARGIN << MARGIN << "Allowable: " << string_or_null(g.allowable()) << endl;
	os << MARGIN << MARGIN << "Max_allowable_len: " << g.max_allowable_len() << endl;
	os << MARGIN << MARGIN << "Before: " << g.before() << endl;
	os << MARGIN << MARGIN << "After: " << g.after() << endl;
}

void dump_slider(std::ostream &os, const ResGadget &gadget)
{
	ResSlider g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_value_changed())	os << MARGIN << MARGIN << " generate_value_changed" << endl;
	if (g.generate_value_changed_while_dragging())	os << MARGIN << MARGIN << " generate_value_changed_while_dragging" << endl;
	if (g.vertical())	os << MARGIN << MARGIN << " vertical" << endl;
	if (g.bar_is_draggable())	os << MARGIN << MARGIN << " bar_is_draggable" << endl;
	if (g.bar_colour())	os << MARGIN << MARGIN << " bar_colour" << endl;
	if (g.background_colour())	os << MARGIN << MARGIN << " background_colour" << endl;
	os << MARGIN << MARGIN << "Lower_bound: " << g.lower_bound() << endl;
	os << MARGIN << MARGIN << "Upper_bound: " << g.upper_bound() << endl;
	os << MARGIN << MARGIN << "Step_size: " << g.step_size() << endl;
	os << MARGIN << MARGIN << "Initial_value: " << g.initial_value() << endl;
}

void dump_draggable(std::ostream &os, const ResGadget &gadget)
{
	ResDraggable g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_drag_started())	os << MARGIN << MARGIN << " generate_drag_started" << endl;
	if (g.has_sprite())	os << MARGIN << MARGIN << " has_sprite" << endl;
	if (g.has_text())	os << MARGIN << MARGIN << " has_text" << endl;
	if (g.draggable_type())	os << MARGIN << MARGIN << " type" << endl;
	if (g.deliver_toolbox_ids())	os << MARGIN << MARGIN << " deliver_toolbox_ids" << endl;
	if (g.drop_shadow())	os << MARGIN << MARGIN << " drop_shadow" << endl;
	if (g.not_dithered())	os << MARGIN << MARGIN << " not_dithered" << endl;
	os << MARGIN << MARGIN << "Text: " << string_or_null(g.text()) << endl;
	os << MARGIN << MARGIN << "Max_text_len: " << g.max_text_len() << endl;
	os << MARGIN << MARGIN << "Sprite: " << string_or_null(g.sprite()) << endl;
	os << MARGIN << MARGIN << "Max_sprite_len: " << g.max_sprite_len() << endl;
}

void dump_popup(std::ostream &os, const ResGadget &gadget)
{
	ResPopUp g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << MARGIN << " generate_about_to_be_shown" << endl;
	os << MARGIN << MARGIN << "Menu: " << string_or_null(g.menu()) << endl;
}

void dump_numberrange(std::ostream &os, const ResGadget &gadget)
{
	ResNumberRange g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_value_changed())	os << MARGIN << MARGIN << " generate_value_change" << endl;
	if (g.writable())	os << MARGIN << MARGIN << " writable" << endl;
	if (g.no_display_area())	os << MARGIN << MARGIN << " no_display_area" << endl;
	if (g.has_adjuster_arrows())	os << MARGIN << MARGIN << " has_adjuster_arrows" << endl;
	if (g.slider_type())	os << MARGIN << MARGIN << " slider_type" << endl;
	os << MARGIN << MARGIN << " Justification : " << g.justification() << endl;
	if (g.slider_colour())	os << MARGIN << MARGIN << " slider_colour" << endl;
	if (g.slider_background_colour())	os << MARGIN << MARGIN << " slider_background_colour" << endl;
	os << MARGIN << MARGIN << "Lower_bound: " << g.lower_bound() << endl;
	os << MARGIN << MARGIN << "Upper_bound: " << g.upper_bound() << endl;
	os << MARGIN << MARGIN << "Step_size: " << g.step_size() << endl;
	os << MARGIN << MARGIN << "Initial_value: " << g.initial_value() << endl;
	os << MARGIN << MARGIN << "Precision: " << g.precision() << endl;
	os << MARGIN << MARGIN << "Before: " << g.before() << endl;
	os << MARGIN << MARGIN << "After: " << g.after() << endl;
	os << MARGIN << MARGIN << "Display_length: " << g.display_length() << endl;
}

void dump_stringset(std::ostream &os, const ResGadget &gadget)
{
	ResStringSet g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_value_changed())	os << MARGIN << MARGIN << " generate_value_changed" << endl;
	if (g.writable())	os << MARGIN << MARGIN << " writable" << endl;
	if (g.generate_event_before_showing_menu())	os << MARGIN << MARGIN << " generate_event_before_showing_menu" << endl;
	if (g.no_value_field())	os << MARGIN << MARGIN << " no_value_field" << endl;
	os << MARGIN << MARGIN << " Justification : " << g.justification() << endl;
	os << MARGIN << MARGIN << "String_set: " << string_or_null(g.string_set()) << endl;
	os << MARGIN << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << MARGIN << "Selected_string: " << string_or_null(g.selected_string()) << endl;
	os << MARGIN << MARGIN << "Max_selected_string: " << g.max_selected_string() << endl;
	os << MARGIN << MARGIN << "Allowable: " << string_or_null(g.allowable()) << endl;
	os << MARGIN << MARGIN << "Max_allowable: " << g.max_allowable() << endl;
	os << MARGIN << MARGIN << "Before: " << g.before() << endl;
	os << MARGIN << MARGIN << "After: " << g.after() << endl;
}

void dump_toolaction(std::ostream &os, const ResGadget &gadget)
{
	ResToolAction g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_select())	os << MARGIN << MARGIN << " generate_select" << endl;
	if (g.has_text())	os << MARGIN << MARGIN << " has_text" << endl;
	if (g.on())	os << MARGIN << MARGIN << " on" << endl;
	if (g.auto_toggle())	os << MARGIN << MARGIN << " auto_toggle" << endl;
	if (g.no_pressed_sprite())	os << MARGIN << MARGIN << " no_pressed_sprite" << endl;
	if (g.auto_repeat())	os << MARGIN << MARGIN << " auto_repeat" << endl;
	if (g.show_transient())	os << MARGIN << MARGIN << " show_transient" << endl;
	if (g.show_as_pop_up())	os << MARGIN << MARGIN << " show_as_pop_up" << endl;
	if (g.has_fade_sprite())	os << MARGIN << MARGIN << " has_fade_sprite" << endl;
	os << MARGIN << MARGIN << "Off_text: " << string_or_null(g.off_text()) << endl;
	os << MARGIN << MARGIN << "Max_off_text: " << g.max_off_text() << endl;
	os << MARGIN << MARGIN << "On_text: " << string_or_null(g.on_text()) << endl;
	os << MARGIN << MARGIN << "Max_on_text: " << g.max_on_text() << endl;
	os << MARGIN << MARGIN << "Click_event: " << g.click_event() << endl;
	os << MARGIN << MARGIN << "Select_show: " << string_or_null(g.select_show()) << endl;
	os << MARGIN << MARGIN << "Adjust_event: " << g.adjust_event() << endl;
	os << MARGIN << MARGIN << "Adjust_show: " << g.adjust_show() << endl;
	os << MARGIN << MARGIN << "Fade_text: " << string_or_null(g.fade_text()) << endl;
	os << MARGIN << MARGIN << "Max_fade: " << g.max_fade() << endl;
}

void dump_textarea(std::ostream &os, const ResGadget &gadget)
{
	ResTextArea g(gadget);

	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.has_vertical_scrollbar())	os << MARGIN << MARGIN << " has_vertical_scrollbar" << endl;
	if (g.has_horizontal_scrollbar())	os << MARGIN << MARGIN << " has_horizontal_scrollbar" << endl;
	if (g.wordwrap())	os << MARGIN << MARGIN << " wordwrap" << endl;
	if (g.allow_selection())	os << MARGIN << MARGIN << " allow_selection" << endl;
	if (g.allow_save())	os << MARGIN << MARGIN << " allow_save" << endl;
	if (g.allow_load())	os << MARGIN << MARGIN << " allow_load" << endl;
	if (g.clipboard())	os << MARGIN << MARGIN << " clipboard" << endl;
	if (g.auto_scroll())	os << MARGIN << MARGIN << " auto_scroll" << endl;
	os << MARGIN << MARGIN << "Type: " << g.type() << endl;
	os << MARGIN << MARGIN << "Event: " << g.event() << endl;
	os << MARGIN << MARGIN << "Text: " << string_or_null(g.text()) << endl;
	os << MARGIN << MARGIN << "Foreground: " << g.foreground() << endl;
	os << MARGIN << MARGIN << "Background: " << g.background() << endl;
}

void dump_scrolllist(std::ostream &os, const ResGadget &gadget)
{
	ResScrollList g(gadget);

	os << MARGIN << MARGIN << "Event: " << g.event() << endl;
	os << MARGIN << MARGIN << "Foreground: " << g.foreground() << endl;
	os << MARGIN << MARGIN << "Background: " << g.background() << endl;
}
