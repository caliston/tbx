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

#include "loadermanager.h"
#include "application.h"
#include "point.h"
#include "path.h"
#include "iconbar.h"
#include "string.h"

namespace tbx {

LoaderManager *LoaderManager::_instance = 0;

LoaderManager::LoaderManager()
{
	_instance = this;

	// Register for messages we handle
	app()->add_message_listener(1, this); // DataSave
	app()->add_message_listener(3, this); // DataLoad
}

LoaderManager::~LoaderManager() {
}

void LoaderManager::add_loader(ObjectId handle, ComponentId id, int file_type, Loader *loader)
{
	LoaderItem *new_item = new LoaderItem();
	new_item->handle = handle;
	new_item->id = id;
	new_item->file_type = file_type;
	new_item->loader = loader;
	new_item->next = 0;

	std::map<ObjectId, LoaderItem *>::iterator found = _loaders.find(handle);
	if (found != _loaders.end())
	{
		LoaderItem *prev = 0;
		LoaderItem *item = found->second;

		while (item
				&& (item->id > id
					|| (item->id == id && item->file_type >= file_type)
				    )
				)
		{
			prev = item;
			item = item->next;
		}
        new_item->next = item;
        if (prev)
        {
        	prev->next = new_item;
        } else
        {
        	_loaders[handle] = new_item;
        }
	} else
	{
		_loaders[handle] = new_item;
	}
}

void LoaderManager::remove_loader(ObjectId handle, ComponentId id, int file_type, Loader *loader)
{
	std::map<ObjectId, LoaderItem *>::iterator found = _loaders.find(handle);
	if (found != _loaders.end())
	{
		LoaderItem *prev = 0;
		LoaderItem *item = found->second;
		while (item
				&& (item->id != id || item->file_type != file_type
					|| item->loader != loader)
				)
		{
			prev = item;
			item = item->next;
		}
		if (item)
		{
			if (prev == 0)
			{
				if (item->next == 0)
				{
					_loaders.erase(found);
				} else
				{
					_loaders[handle] = item->next;
				}
			} else
			{
				prev->next = item->next;
			}
			delete item;
		}
	}
}

void LoaderManager::remove_all_loaders(ObjectId handle)
{
	std::map<ObjectId, LoaderItem *>::iterator found = _loaders.find(handle);
	if (found != _loaders.end())
	{
		LoaderItem *item = found->second;
		LoaderItem *next;
		while (item)
		{
			next = item->next;
			delete item;
			item = next;
		}
		_loaders.erase(found);
	}
}

void LoaderManager::remove_all_loaders(ObjectId handle, ComponentId id)
{
	std::map<ObjectId, LoaderItem *>::iterator found = _loaders.find(handle);
	if (found != _loaders.end())
	{
		LoaderItem *prev = 0;
		LoaderItem *item = found->second;
		while (item  && item->id != id)
		{
			prev = item;
			item = item->next;
		}
		if (item && item->id == id)
		{
			LoaderItem *next;

			while (item && item->id == id)
			{
				next = item->next;
				delete item;
				item = next;
			}

			if (prev == 0)
			{
				if (item == 0)
				{
					_loaders.erase(found);
				} else
				{
					_loaders[handle] = item;
				}
			} else
			{
				prev->next = item;
			}
		}
	}
}


void LoaderManager::user_message(WimpMessageEvent &event)
{
    switch(event.message().message_id())
    {
    case 3: // Message DataLoad - should be sent recorded (see below, but
    	    // PRM says must allow for both types.
    	if (data_message(event.message(), true))
    	{
    		if (strcasecmp(event.message().str(11), "<Wimp$Scrap>") == 0)
    		{
    			Path path("<Wimp$Scrap>");
    			path.remove();
    		}
    		event.claim();
    	}
    	break;
    }
}

void LoaderManager::user_message_recorded(WimpMessageEvent &event, int reply_to)
{
    switch(event.message().message_id())
    {
    case 1: // Message DataSave - file coming from another application
    	if (data_message(event.message(), false))
    	{
    		WimpMessage reply(event.message(), 12);
    		reply.message_id(2); // DataSaveAck
    		reply.your_ref(event.message().my_ref());
    		strcpy(reply.str(11), "<Wimp$Scrap>");

    		reply.send(WimpMessage::Acknowledge, reply_to);
    		event.claim();
    	}
    	break;

    case 3: // Message DataLoad
    	if (data_message(event.message(), true))
    	{
    		if (strcasecmp(event.message().str(11), "<Wimp$Scrap>") == 0)
    		{
    			Path path("<Wimp$Scrap>");
    			path.remove();
    		}

    		WimpMessage reply(event.message());
    		reply.message_id(4); // DataLoadAck
    		reply.your_ref(event.message().my_ref());

    		reply.send(WimpMessage::Acknowledge, reply_to);
    		event.claim();
    	}
    	break;
    }
}

void LoaderManager::user_message_acknowledge(WimpMessageEvent &event)
{
}

/**
 * Received data load message
 *
 * @param msg the wimp message for the transfer.
 * @param load true to load the file, false to just check it can be accepted.
 * @return true if file is loaded
 */
bool LoaderManager::data_message(const WimpMessage &msg, bool load)
{
	int dest_window = msg[5];
	int dest_icon   = msg[6];

	Object load_object;
	Gadget load_gadget;
	bool done = false;

	if (dest_window == -2)
	{
		std::map<ObjectId, LoaderItem *>::iterator i;
		for (i = _loaders.begin(); i != _loaders.end(); ++i)
		{
			Object obj(i->first);
			if (obj.toolbox_class() == Iconbar::TOOLBOX_CLASS
					&& Iconbar(obj).icon_handle() == dest_icon)
			{
				load_object = obj;
				break;
			}
		}
	} else
	{
		Window window;
		load_gadget = Window::from_handle(dest_window, dest_icon, &window);
		load_object = window;
	}

	if (!load_object.null())
	{
		std::map<ObjectId, LoaderItem *>::iterator found = _loaders.find(load_object.handle());
		if (found != _loaders.end())
		{
			LoaderItem *item = found->second;
			ComponentId id = load_gadget.id();
			LoadEvent event(load_object, load_gadget, msg[7], msg[8], msg[9], msg[10], msg.str(11), (msg.your_ref() == 0));
			while (item && !done)
			{
				if (item->id == id || item->id == NULL_ComponentId)
				{
					if (item->file_type == event.file_type() || item->file_type == -2)
					{
						done = item->loader->accept_file(event);
						if (done) done = item->loader->load_file(event);
					}
				}
				item = item->next;
			}
		}
	}

	return done;
}
}
