#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include "GUI/guiserv.h"
#include "GUI/graphics.h"

unsigned char VideoRam[SCREEN_TOTAL];

	Window
create_simple_window(Display* display, int width, int height, int x, int y)
{
	int screen_num = DefaultScreen(display);
	int win_border_width = 2;
	Window win;

	win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
			x, y, width, height, win_border_width,
			BlackPixel(display, screen_num),
			WhitePixel(display, screen_num));

	XMapWindow(display, win);

	XFlush(display);

	return win;
}

	GC
create_gc(Display* display, Window win, int reverse_video)
{
	GC gc;
	unsigned long valuemask = 0;
	XGCValues values;
	unsigned int line_width = 2;
	int line_style = LineSolid;
	int cap_style = CapButt;
	int join_style = JoinBevel;
	int screen_num = DefaultScreen(display);

	gc = XCreateGC(display, win, valuemask, &values);
	if (gc < 0) {
		fprintf(stderr, "XCreateGC: \n");
	}

	if (reverse_video) {
		XSetForeground(display, gc, WhitePixel(display, screen_num));
		XSetBackground(display, gc, BlackPixel(display, screen_num));
	}
	else {
		XSetForeground(display, gc, BlackPixel(display, screen_num));
		XSetBackground(display, gc, WhitePixel(display, screen_num));
	}

	XSetLineAttributes(display, gc,
			line_width, line_style, cap_style, join_style);

	XSetFillStyle(display, gc, FillSolid);

	return gc;
}

	void
draw_screen(Display* display, GC gc, GC rev_gc, Drawable window)
{
	int x = 0, y = 0;
	for (y = 0; y < SCREEN_HEIGHT; ++y)
		for (x = 0; x < SCREEN_WIDTH; ++x)
			XDrawPoint(display, window, IsPixelSet(x, y) ? gc : rev_gc, x, y);
}

	void
handle_expose(Display* display, GC gc, GC rev_gc, XExposeEvent* expose_event)
{
	if (expose_event->count != 0)
		return;
	draw_screen(display, gc, rev_gc, expose_event->window);
}

	void
handle_button_down(Display* display, GC gc, GC rev_gc, XButtonEvent* button_event)
{
	int x, y;
	struct CEventDescriptor event;

	event.Type	= EVENT_TOUCHSCREEN;
	event.x		= button_event->x;
	event.y		= button_event->y;

	while (!ProcessGUIServer(&event))
		;
	event.Type = EVENT_NULL;
	while (!ProcessGUIServer(&event))
		;
	draw_screen(display, gc, rev_gc, button_event->window);
}

	int
handle_key_down(Display* display, GC gc, GC rev_gc, XKeyEvent* key_event)
{
	struct CEventDescriptor event;
	event.Type = EVENT_KEYBOARD;

	switch (key_event->keycode)
	{
	case 9: // esc
		return 1;
	case 100:
		event.key = K_LEFT;
		break;
	case 102:
		event.key = K_RIGHT;
		break;
	case 98:
		event.key = K_UP;
		break;
	case 104:
		event.key = K_DOWN;
		break;
	case 19:
	case 90:
		event.key = K_0;
		break;
	case 10:
	case 87:
		event.key = K_1;
		break;
	case 11:
	case 88:
		event.key = K_2;
		break;
	case 12:
	case 89:
		event.key = K_3;
		break;
	case 13:
	case 83:
		event.key = K_4;
		break;
	case 14:
	case 84:
		event.key = K_5;
		break;
	case 15:
	case 85:
		event.key = K_6;
		break;
	case 16:
	case 79:
		event.key = K_7;
		break;
	case 17:
	case 80:
		event.key= K_8;
		break;
	case 18:
	case 81:
		event.key = K_9;
		break;
	case 105:
		event.key = K_PGDOWN;
		break;
	case 99:
		event.key = K_PGUP;
		break;
	case 21:
	case 86:
		event.key = K_INC;
		break;
	case 20:
	case 82:
		event.key = K_DEC;
		break;
	default:
		return 0;
	}
	while (!ProcessGUIServer(&event))
		;
	event.Type = EVENT_NULL;
	while (!ProcessGUIServer(&event))
		;
	draw_screen(display, gc, rev_gc, key_event->window);
	return 0;
}

	int
main(int argc, char* argv[])
{
	Display* display;
	Window win;
	char *display_name = getenv("DISPLAY");
	GC gc, rev_gc;
	struct CEventDescriptor event =
	{
		.Type = EVENT_NULL
	};

	InitGUIServer();
	ClearScreen();
	while (!ProcessGUIServer(&event))
		;

	display = XOpenDisplay(display_name);
	if (display == NULL) {
		fprintf(stderr, "%s: cannot connect to X server '%s'\n",
				argv[0], display_name);
		exit(1);
	}

	win = create_simple_window(display, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	gc = create_gc(display, win, 0);
	rev_gc = create_gc(display, win, 1);

	XSelectInput(display, win, ExposureMask | KeyPressMask |
			ButtonPressMask | StructureNotifyMask);

	{
		int done = 0;
		XEvent an_event;
		while (!done) {
			XNextEvent(display, &an_event);
			switch (an_event.type) {
				case Expose:
					handle_expose(display, gc, rev_gc, (XExposeEvent*)&an_event.xexpose);
					break;

				case ButtonPress:
					handle_button_down(display, gc, rev_gc,
							(XButtonEvent*)&an_event.xbutton);
					break;

				case KeyPress:
					done = handle_key_down(display, gc, rev_gc,
							(XKeyEvent*)&an_event.xkey);
					break;

				default:
					break;
			}
		}
	}

	XFreeGC(display, gc);
	XFreeGC(display, rev_gc);

	XCloseDisplay(display);
	return 0;
}

