#include <pebble.h>

static Window *s_main_window;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;


// Get the current date and time
static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char s_timebuffer[8];
  static char s_datebuffer[14];
  
  // Set time
	strftime(s_timebuffer, sizeof(s_timebuffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
	text_layer_set_text(s_time_layer, s_timebuffer);
  // Set date
	strftime(s_datebuffer, sizeof(s_datebuffer), "%a %m/%d/%y", tick_time);
	text_layer_set_text(s_date_layer, s_datebuffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

// Creation of Window elements
static void main_window_load(Window *window) {
	// Create image layer

	// Get info for Window, Image, and TextLayer
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
  s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ESPRESSO);
	s_image_layer = bitmap_layer_create(bounds);
	s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(88, 82), bounds.size.w, 72));
  s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(128, 122), bounds.size.w, 48));
  
	// Set Layer
	bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
	bitmap_layer_set_alignment(s_image_layer, GAlignTop);
	bitmap_layer_set_background_color(s_image_layer, GColorBlack);
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_36_BOLD_NUMBERS));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorLightGray);
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
	// Add created layers as child layer
	layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}
static void main_window_unload(Window *window){
	gbitmap_destroy(s_image_bitmap);
	bitmap_layer_destroy(s_image_layer);
	text_layer_destroy(s_time_layer);
}

static void init() {
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	window_stack_push(s_main_window, true);
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}