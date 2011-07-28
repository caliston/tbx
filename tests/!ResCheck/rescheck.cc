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
#include "tbx/res/resscale.h"
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

#include "tbx/res/resfile.h"

#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace tbx::res;

char *master_folder = "<ResCheck$Dir>.master.";
char *test_folder = "<ResCheck$Dir>.test.";

bool compare_resources(const char *test_name, const std::string &source_fname, const std::string &target_fname);
void where_in_res(std::ostream &os, char *here, char *start);

void save_test();
void copy_test();
void copy_writable_test();
void default_object_test();
void default_gadget_test();
void shortcut_test();
void readonly_test();

/**
 * Main entry point
 */
int main()
{
	save_test();
	copy_test();
	copy_writable_test();
	default_object_test();
	default_gadget_test();
	shortcut_test();
	readonly_test();

	return 0;
}


/**
 * Load and save and check the same
 */
void save_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "Res";
	target_fname += "ResSave";

	ResEditor editor;
	if (!editor.load(source_fname))
	{
		cout << "save_test: Failed: Unable to load " << source_fname << endl;
		return;
	}

	if (!editor.save(target_fname))
	{
		cout << "save_test: Failed: Unable to save " << target_fname << endl;
		return;
	}

	if (compare_resources("save_test", source_fname, target_fname))
	{
		cout << "save_test: OK" << endl;
	}
}

/**
 * Copy from editor to new editor.
 * Should just save the same objects in the original editor.
 */
void copy_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "Res";
	target_fname += "ResCopy";

	ResEditor editor;
	if (!editor.load(source_fname))
	{
		cout << "copy_test: Failed: Unable to load " << source_fname << endl;
		return;
	}

	ResEditor copy;

	for (ResEditor::const_iterator i = editor.begin(); i != editor.end(); ++i)
	{
		copy.add(*i);
	}

	if (!copy.save(target_fname))
	{
		cout << "copy_test: Failed: Unable to save " << target_fname << endl;
		return;
	}

	if (compare_resources("copy_test", source_fname, target_fname))
	{
		cout << "copy_test: OK" << endl;
	}
}

/**
 * Copy from editor to new editor.
 * Ensure a copy of the object is added to the copy
 * by making each object writeable before adding it.
 */
void copy_writable_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "Res";
	target_fname += "ResCopyW";

	ResEditor editor;
	if (!editor.load(source_fname))
	{
		cout << "copy_writable_test: Failed: Unable to load " << source_fname << endl;
		return;
	}

	ResEditor copy;

	for (ResEditor::const_iterator i = editor.begin(); i != editor.end(); ++i)
	{
		ResObject obj = *i;
		bool shared = obj.shared();
		obj.shared(!shared); // Force it to make a new copy of the object body and data
		obj.shared(shared);
		copy.add(obj);
	}

	if (!copy.save(target_fname))
	{
		cout << "copy_writable_test: Failed: Unable to save " << target_fname << endl;
		return;
	}

	if (compare_resources("copy_writable_test", source_fname, target_fname))
	{
		cout << "copy_writable_test: OK" << endl;
	}
}

/**
 * Check default constructors create objects expected
 */
void default_object_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "ResDefault";
	target_fname += "ResDefTest";

	ResEditor editor;
	ResColourDbox colour_dbox("ColourDbox");
	editor.add(colour_dbox);

	// Need to mess about with name as the letter "o" is in the padding
	ResWindow window("Windowxo");
	window.name("Window");
	editor.add(window);
	ResWindow toolbar("Toolbar", true);
	editor.add(toolbar);

	ResColourMenu colour_menu("ColourMenu");
	editor.add(colour_menu);
	ResFileInfo fi("FileInfo");
	fi.date(0x455732efb6LL); // Have to match date to resource as it's not defaulted
	editor.add(fi);
	ResFontDbox fontdbox("FontDbox");
	editor.add(fontdbox);
	ResFontMenu fontmenu("FontMenu");
	editor.add(fontmenu);

	ResIconbar iconbar("Iconbar");
	// Match test resource
	iconbar.sprite_name("!resed");
	iconbar.text("iconbar");
	editor.add(iconbar);
	ResSaveAs saveas("SaveAs");
	editor.add(saveas);
	ResScale scale("Scale");
	editor.add(scale);
	ResPrintDbox printdbox("PrintDbox");
	editor.add(printdbox);
	ResDCS dcs("DCS");
	editor.add(dcs);
	ResQuit quit("Quit");
	editor.add(quit);
	ResProgInfo proginfo("ProgInfo", true);
	// Default doesn't set details - so set to match test resource
	proginfo.author("© Acorn Computers Ltd, 1994");
	proginfo.version("0.01 (dd-mmm-yy)");
	proginfo.uri("http://www.acorn.com/");
	editor.add(proginfo);

	ResMenu menu("Menu");
	// Match text in resource and add a sample menu item
	menu.title("New Menu");

	// Add the default menu item in the resource
	ResMenuItem menu_item;
	menu_item.text("Menu Entry"); // Match text in resource
	menu.add(menu_item);

	editor.add(menu);

	if (!editor.save(target_fname))
	{
		cout << "default_object_test: Failed: Unable to save " << target_fname << endl;
		return;
	}

	if (compare_resources("default_object_test", source_fname, target_fname))
	{
		cout << "default_object_test: OK" << endl;
	}
}

void default_gadget_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "ResDefGadgets";
	target_fname += "ResDefGadgetsTest";

	ResEditor editor;
	// Need to mess about with name as the letter "o" is in the padding
	ResWindow window("Windowxo");
	window.name("Window");
	window.visible_ymin(0x2e0);
	window.visible_ymax(0x488);

	ResAdjuster adj; // 164
	adj.increment(true); // Match resource default
	adj.xmin(476);
	adj.ymin(-160);
	adj.xmax(508);
	adj.ymax(-128);
	window.add_gadget(adj);

	ResPopUp popup; // 204
	popup.xmin(464);
    popup.ymin(-224);
    popup.xmax(508);
    popup.ymax(-180);
    popup.component_id(1);
	window.add_gadget(popup);

	ResSlider slider; // 244
	slider.xmin(528);
    slider.ymin(-272);
    slider.xmax(568);
    slider.ymax(-16);
    slider.component_id(2);
	slider.vertical(true);
	slider.initial_value(50);
	window.add_gadget(slider);

	ResSlider slider2; // 296
	slider2.xmin(252);
    slider2.ymin(-276);
    slider2.xmax(508);
    slider2.ymax(-236);
    slider2.component_id(3);
	slider2.initial_value(50);
	window.add_gadget(slider2);

	ResDraggable draggable; // 348
	draggable.xmin(280);
    draggable.ymin(-396);
    draggable.xmax(436);
    draggable.ymax(-292);
    draggable.component_id(4);
	draggable.has_text(true);
	draggable.text("Draggable");
	draggable.has_sprite(true);
	draggable.sprite("file_fae");
	window.add_gadget(draggable);

	ResButton button; // 400
	button.xmin(448);
    button.ymin(-396);
    button.xmax(564);
    button.ymax(-292);
    button.component_id(5);
	button.vcentred(true);
	button.hcentred(true);
	button.has_border(true);
	button.has_text(true);
	button.value("Button", 8);
	button.validation(0, 1);
	window.add_gadget(button);

	ResStringSet stringset; // 456
	stringset.xmin(24);
    stringset.ymin(-392);
    stringset.xmax(268);
    stringset.ymax(-340);
    stringset.component_id(6);
	stringset.justification(CENTRE); // centered
	stringset.string_set("Item 1,Item 2");
	stringset.title("Items");
	stringset.selected_string("String set");
	window.add_gadget(stringset);

	ResNumberRange numberrange; // 524
	numberrange.xmin(24);
    numberrange.ymin(-328);
    numberrange.xmax(212);
    numberrange.ymax(-276);
    numberrange.component_id(7);
	numberrange.has_adjuster_arrows(true);
	numberrange.justification(RIGHT);
	numberrange.upper_bound(10000);
	numberrange.initial_value(4999);
	numberrange.precision(2);
	numberrange.display_length(116);
	window.add_gadget(numberrange);

	ResWritableField writable; // 592
	writable.xmin(24);
    writable.ymin(-264);
    writable.xmax(212);
    writable.ymax(-212);
    writable.component_id(8);
	writable.justification(CENTRE);
	writable.text("Writable");
	window.add_gadget(writable);

	ResDisplayField display; // 652
	display.xmin(24);
    display.ymin(-200);
    display.xmax(212);
    display.ymax(-148);
    display.component_id(9);
	display.justification(CENTRE);
	display.text("Display");
	window.add_gadget(display);

	ResActionButton action; // 696
	action.xmin(24);
    action.ymin(-136);
    action.xmax(212);
    action.ymax(-84);
    action.component_id(10);
	action.text("Action");
	window.add_gadget(action);

	ResLabel label; // 748
	label.xmin(24);
    label.ymin(-72);
    label.xmax(212);
    label.ymax(-20);
    label.component_id(11);
	label.label("Label");
	label.omit_bounding_box(true);
	label.justification(RIGHT);
	window.add_gadget(label);

	ResRadioButton radio; // 788
	radio.xmin(248);
    radio.ymin(-224);
    radio.xmax(436);
    radio.ymax(-180);
    radio.component_id(12);
	radio.on(true);
	radio.group_number(2);
	radio.label("Radio");
	window.add_gadget(radio);

	ResOptionButton option; // 840
	option.xmin(248);
    option.ymin(-168);
    option.xmax(436);
    option.ymax(-124);
    option.component_id(13);
	option.label("Option");
	window.add_gadget(option);

	ResLabelledBox labelledbox; // 888
	labelledbox.xmin(248);
    labelledbox.ymin(-104);
    labelledbox.xmax(516);
    labelledbox.ymax(-20);
    labelledbox.component_id(14);
	labelledbox.label("Labelled box");
	window.add_gadget(labelledbox);

	ResScrollList scrolllist; // 928
	scrolllist.xmin(24);
    scrolllist.ymin(-532);
    scrolllist.xmax(232);
    scrolllist.ymax(-420);
    scrolllist.component_id(15);
	window.add_gadget(scrolllist);

	ResTextArea textarea; // 976
	textarea.xmin(264);
    textarea.ymin(-616);
    textarea.xmax(564);
    textarea.ymax(-420);
    textarea.component_id(16);
	textarea.has_vertical_scrollbar(true);
	textarea.text("TextArea");
	window.add_gadget(textarea);

	ResToolAction toolaction; // 1032
	toolaction.xmin(24);
    toolaction.ymin(-616);
    toolaction.xmax(232);
    toolaction.ymax(-552);
    toolaction.component_id(17);
	toolaction.has_text(true);
	toolaction.generate_select(true);
	toolaction.off_text("ToolAction");
	window.add_gadget(toolaction);

	editor.add(window);

	if (!editor.save(target_fname))
	{
		cout << "default_gadget_test: Failed: Unable to save " << target_fname << endl;
		return;
	}

	if (compare_resources("default_gadget_test", source_fname, target_fname))
	{
		cout << "default_gadget_test: OK" << endl;
	}
}

void shortcut_test()
{
	std::string source_fname(master_folder);
	std::string target_fname(test_folder);
	source_fname += "ResShortcuts";
	target_fname += "ResShortcutsTest";

	ResEditor editor;

	// Window with just shortcut
	// Need to mess about with name as the letter "o" is in the padding
	ResWindow window("Windowxo");
	window.name("Window");
	window.visible_ymin(0x2e0);
	window.visible_ymax(0x488);

	ResShortcut a('a');
	a.event_id(1);
	ResShortcut b('b');
	b.show("b");

	window.add_shortcut(a);
	window.add_shortcut(b);

	editor.add(window);

	// Label for gadget and shortcut tests
	ResLabel label;
	label.label("Label");

	// Window with label added, then shortcut
	ResWindow w1("Window_1");
	w1.add_gadget(label);
	w1.add_shortcut(a);
	w1.add_shortcut(b);
	editor.add(w1);

	// Window shortcut added, then label
	ResWindow w2("Window_2");
	w2.add_shortcut(a);
	w2.add_shortcut(b);
	w2.add_gadget(label);
	editor.add(w2);

	// All we can test is that the we don't get crashes as ResEdit put the
	// shortcuts at the beginning of the table padded for some reason and
	// we tbx doesn't.

	cout << "shortcut_test: OK" << endl;
}


void readonly_test()
{
	std::string source_fname(master_folder);
	source_fname += "ResDefault";
	ResFile rf;
	if (!rf.load(source_fname))
	{
		cout << "readonly_test: Failed: Unable to load " << source_fname << endl;
		return;
	}

	try
	{
		ResMenu menu = rf.object("Menu");
		ResMenuItem item = menu.item(0);

		std::string text1 = "This is first text";
		std::string text2 = ""; // This is second text thats longer";

		for (int j = 0; j < 300; j++)
		{
			if (j&1)
			{
				text2 += 'a';
				item.text(text2);
			} else item.text(text1);
			item.component_id(j);
		}

//		item.text("New text for the string");

		cout << "readonly_test: OK" << endl;
	} catch(std::exception &e)
	{
		cout << "readonly_text: Failed: " << e.what() << endl;
	}
}

/**
 * Compare to resource files and report differences
 */
bool compare_resources(const char *test_name, const std::string &master_fname, const std::string &check_fname)
{
	ifstream master(master_fname.c_str(), ios::binary), check(check_fname.c_str(), ios::binary );
	if (!master)
	{
		cout << test_name << ": Failed: Unable to load master file " << master_fname << endl;
		return false;
	}
	if (!check)
	{
		cout << test_name << ": Failed: Unable to load check file " << check_fname << endl;
		return false;
	}

	master.seekg (0, ios::end);
	int master_length = master.tellg();
	master.seekg (0, ios::beg);

	if (master_length == 0)
	{
		cout << test_name << ": Failed: Master file is empty" << master_fname << endl;
		return false;
	}

	check.seekg (0, ios::end);
	int check_length = check.tellg();
	check.seekg (0, ios::beg);

	if (check_length == 0)
	{
		cout << test_name << ": Failed: check file is empty" << check_fname << endl;
		return false;
	}

	if (master_length != check_length)
	{
		cout << test_name << ": Failed: file lengths are not the same" << endl;
		cout << test_name << ": Failed: Master " << master_length << " (" << master_fname << ")" << endl;
		cout << test_name << ": Failed: Check  " << check_length << " ( " << check_fname << ")" << endl;
	}

	char *master_data = new char[master_length];
	char *check_data = new char[check_length];
	master.read(master_data, master_length);
	check.read(check_data, check_length);

	int left = min(master_length, check_length);
	bool has_diff = false;

	int *master_ptr = (int *)master_data;
	int *check_ptr = (int *)check_data;
	while (left > 0)
	{
		if (*master_ptr != *check_ptr)
		{
			if (!has_diff)
			{
				cout << test_name << ": Failed: files are different" << endl;
				cout << "    Offset    Master     Check" << hex << endl;
				has_diff = true;
			}
			cout << setw(10) << (char *)master_ptr - master_data
				 << setw(10) << *master_ptr
				 << setw(10) << *check_ptr
				 << " ";
			where_in_res(cout, (char *)master_ptr, master_data);
			cout << endl;
		}
		master_ptr++;
		check_ptr++;
		left -= 4;
	}

	delete [] master_data;
	delete [] check_data;

	return !has_diff && (master_length == check_length);
}

/**
 * Textual description of location in resource file
 */
void where_in_res(std::ostream &os, char *here, char *start)
{
	if (here == start) os << "Resource file header";
	else if (here == start + 4) os << "Resource file version";
	else if (here == start + 8) os << "First object offset";
	else
	{
		int object_offset = *((int *)(start + 8));
		char *pos = start + object_offset;
		if (here < pos) os << "Before first object at " << object_offset;
		else
		{
			char *next_obj_pos = pos;
			ResDataHeader *rdh;
			ResObjectHeader *obj;

			do
			{
				pos = next_obj_pos;
				rdh = (ResDataHeader *)pos;
				obj = (ResObjectHeader *)(pos + 12);

				if (rdh->relocations_table_offset != -1)
				{
					next_obj_pos = pos + rdh->relocations_table_offset;
					next_obj_pos += (*((int *)next_obj_pos)) * 8 + 4;
				} else
				{
					next_obj_pos += 12 + obj->total_size;
				}
			} while (here >= next_obj_pos);

			os << "Object '" << obj->name << "' ";
			if (here < (char *)obj)
			{
				if (here == pos) os << "String table offset";
				else if (here == pos + 4) os << "Message table offset";
				else if (here == pos + 8) os << "Relocation table offset";
			} else
			{
				if (here == (char *)&obj->class_id) os << "Class id";
				else if (here == (char *)&obj->flags) os << "Flags";
				else if (here == (char *)&obj->version) os << "Version";
				else if (here == obj->name
					|| here == obj->name + 4
					|| here == obj->name + 8
					) os << "Name";
				else if (here == (char *)&obj->total_size) os << "Total size";
				else if (here == (char *)&obj->body) os << "Body";
				else if (here == (char *)&obj->body_size) os << "Body size";
				else if (here < (char *)obj + (int)obj->body + obj->body_size)
				{
					int body_offset = here - (char *)obj - (int)obj->body;
					os << "In body at offset " << body_offset << " (" << dec << body_offset << hex << ")";
				} else if (rdh->relocations_table_offset != -1)
				{
					int table_offset = here - pos;
					if (table_offset == rdh->relocations_table_offset)
					{
						os << "Number of relocations" << endl;
					} else if (table_offset > rdh->relocations_table_offset)
					{
						table_offset -= rdh->relocations_table_offset - 4;
						os << "Reloc " << ((table_offset & 4) ? "type  " : "offset")
						<< " at " << table_offset;
						int entry = table_offset / 8;
						os << " entry " << dec << entry << hex;
					} else if (rdh->messages_table_offset != -1
						&& table_offset >= rdh->messages_table_offset)
					{
						os << "Message table offset " << table_offset - rdh->messages_table_offset;
					} else if (rdh->string_table_offset != -1)
					{
						os << "String table offset " << table_offset - rdh->string_table_offset;
					}
				} else
				{
					os << "Can't figure out where offset " << here - (char *)obj;
				}
			}
		}
	}
}
