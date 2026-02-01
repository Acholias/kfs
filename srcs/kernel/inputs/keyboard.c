#include "keyboard.h"
#include "io.h"

static const char	*printable_lowercase = ""
"abcdefghijklmnopqrstuvwxyz"
"1234567890"
"`-=[]\\;',./ "
"0123456789+-*/.";

static const char	*printable_uppercase = ""
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"!@#$%^&*()"
"~_+{}|:\"<>? "
"0123456789+-*/.";

static t_key	scancode_to_key(uint8_t scancode, bool num_lock);
static char		key_to_char(t_key key, bool uppercase);

t_keyboard	keyboard_init(void)
{
	t_keyboard	keyboard;

	keyboard.caps_lock = false;
	keyboard.num_lock = false;
	keyboard.Lshift = false;
	keyboard.Rshift = false;
	keyboard.Lctrl = false;
	keyboard.Rctrl = false;
	keyboard.Lalt = false;
	keyboard.Ralt = false;
	keyboard.super = false;
	keyboard.uppercase = false;

	return (keyboard);
}

bool	keyboard_poll(t_keyboard *keyboard, t_key_event *key_event)
{
	uint8_t status = inb(PS2_STATUS); // Read keyboard status

	// Check if there is data
	if (!(status & 0x01))
		return (false); // If no data, return false

	// If data, read the data
	key_event->scancode = inb(PS2_DATA);

	// Check the type of the event
	if ((key_event->scancode & KEY_UP_MASK) == KEY_UP_MASK)
		key_event->type = KEY_RELEASE;
	else
		key_event->type = KEY_PRESS;

	// Compute the key from the scancode
	key_event->key = scancode_to_key(key_event->scancode, keyboard->num_lock);

	// Compute the ascii of the key
	key_event->ascii = key_to_char(key_event->key, keyboard->uppercase);

	// Update the keyboard state
	if (key_event->ascii == '\0')
	{
		if (key_event->key == K_CAPS_LOCK)
		{
			if (key_event->type)
			{
				keyboard->caps_lock = ~keyboard->caps_lock;
				keyboard->uppercase = keyboard->Lshift || keyboard->Rshift || keyboard->caps_lock;
			}
		}
		else if (key_event->key == K_NUM_LOCK)
		{
			if (key_event->type)
				keyboard->num_lock = ~keyboard->num_lock;
		}
		else if (key_event->key == K_LSHIFT)
		{
			keyboard->Lshift = (bool)key_event->type;
			keyboard->uppercase = keyboard->Lshift || keyboard->Rshift || keyboard->caps_lock;
		}
		else if (key_event->key == K_RSHIFT)
		{
			keyboard->Rshift = (bool)key_event->type;
			keyboard->uppercase = keyboard->Lshift || keyboard->Rshift || keyboard->caps_lock;
		}
		else if (key_event->key == K_LCTRL)
			keyboard->Lctrl = (bool)key_event->type;
		else if (key_event->key == K_RCTRL)
			keyboard->Rctrl = (bool)key_event->type;
		else if (key_event->key == K_LALT)
			keyboard->Lalt = (bool)key_event->type;
		else if (key_event->key == K_RALT)
			keyboard->Ralt = (bool)key_event->type;
		else if (key_event->key == K_SUPER)
			keyboard->super = (bool)key_event->type;
	}

	return (true); // Return true to tell that there is a key event
}


static t_key	scancode_to_key(uint8_t scancode, bool num_lock)
{
	if (scancode > 0x80) // Remove diff from press/release
		scancode -= 0x80;

	if (scancode > 0x58) // Security
		return (K_NONE);

	if (num_lock)
		return (scancode_key_num_lock[scancode]);
	return (scancode_key_num_unlock[scancode]);
}


static char	key_to_char(t_key key, bool uppercase)
{
	// Check if printable
	if (K_A <= key && key <= K_KP_DOT)
	{
		if (uppercase)
			return (printable_uppercase[key - K_A]);
		return (printable_lowercase[key - K_A]);
	}
	return ('\0');
}
