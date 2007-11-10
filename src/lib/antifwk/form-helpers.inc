<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Form helpers.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 *
 */
function has_int_fields($ary, $fields) {
	foreach ($fields as $field_name) {
		if (!array_key_exists($field_name, $ary) || !ctype_digit($ary[$field_name])) {
			return false;
		}
	}
	return true;
}

/**
 * Checks if a given checkbox/radiobutton in a scope is active or not.
 *
 * @param  $scope  Scope (i.e. $_REQUEST/$_GET/$_POST) to check.
 * @param  $name   Name of the checkbox/radiobutton group to check.
 * @param  $value  Value of the checkbox if active.
 *
 * @return TRUE if active, else FALSE.
 */
function is_checked($scope, $name, $value) {
	return isset($scope[$name]) && array_search($value, $scope[$name]) !== false;
}

/**
 * Saves the named POSTed parameters to cookies.
 */
function save_params() {
	$names = func_get_args();
	$time  = time() + make_timespan(0, 0, 0, 365);
	$path  = dirname($_SERVER['PHP_SELF']);
	foreach ($names as $name) {
		if (isset($_POST[$name])) {
			$value = $_POST[$name];
			if (is_array($value)) {
				$value = join(',', $value);
			}
			setcookie('c_' . $name, $value, $time, $path);
		}
	}
}

/**
 * Sets the value of a parameter in the $_REQUEST scope if unset, checking
 * if the value's previously been saved to a cookie first (with save_params()),
 * and then using a default value if there was no appropriate cookie.
 *
 * @param  $name   Parameter name.
 * @param  $value  Default value if unset and there's no appropriate cookie.
 */
function set_default($name, $value='') {
	if (!isset($_REQUEST[$name])) {
		if (isset($_COOKIE['c_' . $name])) {
			if (is_array($value)) {
				$_REQUEST[$name] = explode(',', $_COOKIE['c_' . $name]);
			} else {
				$_REQUEST[$name] = $_COOKIE['c_' . $name];
			}
		} else {
			$_REQUEST[$name] = $value;
		}
	}
}

/**
 * Form helper generating a checkbox.
 *
 * @param  $name     Name of checkbox group.
 * @param  $value    Value corresponding to this checkbox.
 * @param  $label    Label text to use (automatically escaped).
 * @param  $checked  Optional parameter specifying whether it should be
 *                   checked; by default, it checks $_REQUEST and uses that.
 */
function checkbox($name, $value, $label, $checked=null) {
	if (is_null($checked)) {
		$checked = is_checked($_REQUEST, $name, $value);
	}
	$enc_value = htmlentities($value);
	echo '<input type="checkbox" name="', $name, '[]" id="';
	echo $name, '_', $enc_value, '" ';
	if ($checked) {
		echo 'checked="checked" ';
	}
	echo 'value="', $enc_value, '"/>&nbsp;';
	echo '<label for="', $name, '_', $enc_value, '">';
	echo htmlentities($label), '</label>';
}

/**
 * Form helper generating a start form tag.
 *
 * @param  $method  HTTP method to use for form; defaults to 'post'.
 * @param  $hidden  Array giving fields to include as hidden fields; if a
 *                  name is given by itself, the value is taken from $_REQUEST,
 *                  but if a name=>value mapping is given, that's used.
 */
function form($method='post', $hidden=array()) {
	echo '<form method="', strtolower($method), '" action="';
	echo htmlentities($_SERVER['PHP_SELF']), '">';
	foreach ($hidden as $name=>$value) {
		if (is_int($name)) {
			# Grab the value from $_REQUEST and write it out.
			$name  = $value;
			$value = $_REQUEST[$name];
		}
		hidden_field($name, $value);
	}
}

/**
 * Form helper to write out a named hidden field.
 *
 * @param  $name   Name of hidden field.
 * @param  $value  Value for field; defaults to an empty string; can be an
 *                 array.
 */
function hidden_field($name, $value='') {
	if (is_array($value)) {
		foreach ($value as $v) {
			hidden_field($name . '[]', $v);
		}
	} else {
		echo '<input type="hidden" name="', htmlentities($name);
		echo '" value="', htmlentities($value), '"/>';
	}
}

/**
 * Form helper to write out a text field.
 *
 * @param  $name   Name (and id) for the field.
 * @param  $value  Value of textbox; pulls the value from $_REQUEST if none
 *                 given.
 */
function textbox($name, $value=null) {
	if (is_null($value)) {
		$value = $_REQUEST[$name];
	}
	echo '<input type="text" name="', htmlentities($name), '" id="';
	echo htmlentities($name), '" value="', htmlentities($value), '" />';
}

?>
