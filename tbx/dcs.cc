/*
 * dcs.cc
 *
 *  Created on: 05-Jun-2009
 *      Author: alanb
 */

#include "dcs.h"
#include "abouttobeshownlistener.h"
#include "hasbeenhiddenlistener.h"

namespace tbx {

/**
 * This event is raised just before the DCS underlying window is
 * about to be shown.
 */
void DCS::add_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	add_listener(0x82a80, listener, about_to_be_shown_router);
}

/**
 * Remove about to be shown listener
 */
void DCS::remove_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	remove_listener(0x82a80, listener);
}

/**
 * This event is raised after the DCS dialog has been completed
 */
void DCS::add_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	add_listener(0x82a83, listener, has_been_hidden_router);
}

/**
 * Remove has been hidden listener
 */
void DCS::remove_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	remove_listener(0x82a83, listener);
}

static void dcs_discard_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
    static_cast<DCSDiscardListener*>(listener)->dcs_discard();
}

/***
 * Add listener to be called when discard is selected from the dialogue
 */
void DCS::add_discard_listener(DCSDiscardListener *listener)
{
	add_listener(0x82a81, listener, dcs_discard_router);
}

void DCS::remove_discard_listener(DCSDiscardListener *listener)
{
	remove_listener(0x82a81, listener);
}

static void dcs_save_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
    static_cast<DCSSaveListener*>(listener)->dcs_save();
}

/***
 * Add listener to be called when save is selected from the dialogue
 */
void DCS::add_save_listener(DCSSaveListener *listener)
{
	add_listener(0x82a82, listener, dcs_save_router);
}

void DCS::remove_save_listener(DCSSaveListener *listener)
{
	remove_listener(0x82a82, listener);
}

static void dcs_cancel_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
    static_cast<DCSCancelListener*>(listener)->dcs_cancel();
}

/***
 * Add listener to be called when cancel is selected from the dialogue
 */
void DCS::add_cancel_listener(DCSCancelListener *listener)
{
	add_listener(0x82a84, listener, dcs_cancel_router);
}

void DCS::remove_cancel_listener(DCSCancelListener *listener)
{
	remove_listener(0x82a84, listener);
}


}
