/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/
#ifndef HAVE_PHP_UI_FONT
#define HAVE_PHP_UI_FONT
#include <ui.h>

#include "php.h"

#include <classes/descriptor.h>
#include <classes/font.h>

zend_object_handlers php_ui_font_handlers;

zend_class_entry *uiDrawTextFont_ce;

zend_object* php_ui_font_create(zend_class_entry *ce) {
	php_ui_font_t *font = 
		(php_ui_font_t*) ecalloc(1, sizeof(php_ui_font_t) + zend_object_properties_size(ce));

	zend_object_std_init(&font->std, ce);

	object_properties_init(&font->std, ce);

	font->std.handlers = &php_ui_font_handlers;

	return &font->std;
}

void php_ui_font_free(zend_object *o) {
	php_ui_font_t *font = php_ui_font_from(o);

	if (font->f) {
		uiDrawFreeTextFont(font->f);
	}

	zend_object_std_dtor(o);
}
ZEND_BEGIN_ARG_INFO_EX(php_ui_font_construct_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, descriptor, UI\\Draw\\Text\\Font\\Descriptor, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Font UI\Draw\Text\Font::__construct(UI\Draw\Text\Font\Descriptor descriptor) */
PHP_METHOD(DrawTextFont, __construct) 
{
	php_ui_font_t *font = php_ui_font_fetch(getThis());
	zval *descriptor = NULL;
	php_ui_descriptor_t *d;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &descriptor, uiDrawTextFontDescriptor_ce) != SUCCESS) {
		return;
	}

	d = php_ui_descriptor_fetch(descriptor);
	
	font->f = uiDrawLoadClosestFont(&d->d);

	if (!font->f) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_font_methods[] = {
	PHP_ME(DrawTextFont, __construct, php_ui_font_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextFont) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text", "Font", php_ui_font_methods);

	uiDrawTextFont_ce = zend_register_internal_class(&ce);
	uiDrawTextFont_ce->create_object = php_ui_font_create;

	memcpy(&php_ui_font_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_font_handlers.offset = XtOffsetOf(php_ui_font_t, std);
	php_ui_font_handlers.free_obj = php_ui_font_free;
	
	return SUCCESS;
} /* }}} */
#endif
