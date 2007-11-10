<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Framework code.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

define('CRLF', "\r\n");

/**
 * Generates the page header.
 *
 * @param  $title         Page title.
 * @param  $content_type  MIME type of the generated content.
 * @param  $charset       Text encoding used on the page.
 */
function head($title='', $content_type='text/html', $charset='UTF-8') {
	global $cfg;
	header('Content-Type: ' . $content_type . '; charset=' . $charset);
	if ($_SERVER['REQUEST_METHOD'] != 'HEAD') {
		include './templates/head.inc';
	}
}

/**
 * Generates the page footer.
 */
function foot() {
	global $cfg;
	if ($_SERVER['REQUEST_METHOD'] != 'HEAD') {
		include './templates/foot.inc';
	}
}

/**
 * Basic replacement for http_build_query() for PHP4 compatibility.
 */
function build_query_string($params) {
	$sep = '';
	$qs  = '';
	foreach ($params as $name=>$value) {
		$qs .= $sep . urlencode($name) . '=' . urlencode($value);
		$sep = '&';
	}
	return $qs;
}

/**
 *
 */
function dump($data) {
	echo '<pre>', htmlentities(var_export($data, true)), '</pre>';
}

/**
 * Entity-encoded echo: encodes all the special characters in it's arguments
 * and echos them.
 */
function eecho() {
	$args = func_get_args();
	echo htmlspecialchars(implode('', $args), ENT_QUOTES);
}

/**
 * Walks a array, fixing magic quotes.
 */
function fix_magic_quotes(&$arr) {
	$keys =& array_keys($arr);
	$n    =  count($keys);

	for ($i = 0; $i < $n; ++$i) {
		$val =& $arr[$keys[$i]];
		if (is_array($val)) {
			fix_magic_quotes($val);
		} else {
			$val = stripslashes($val);
		}
	}
}

/**
 * Fixes the superglobals by removing any magic quotes, if present.
 */
function fix_superglobals() {
	if (ini_get('magic_quotes_gpc')) {
		fix_magic_quotes($_GET);
		fix_magic_quotes($_POST);
		fix_magic_quotes($_COOKIE);
		fix_magic_quotes($_REQUEST);
	}
}

/**
 *
 */
function get_request_method() {
	$method = strtolower($_SERVER['REQUEST_METHOD']);
	if ($method == 'head') {
		return 'get';
	}
	return $method;
}

/**
 *
 */
function get_action_path() {
	if (array_key_exists('action', $_REQUEST)) {
		$action = $_REQUEST['action'];
	} else {
		$action = '';

		# Look for a parameter starting with '__'. The rest of its name
		# will be used as the action.
		foreach ($_REQUEST as $name=>$value) {
			if (substr($name, 0, 2) == '__') {
				$action = substr($name, 2);
				break;
			}
		}

		if ($action == '') {
			$action = basename($_SERVER['PHP_SELF'], '.php');
		}
	}

	# Make the action safe.
	$action = preg_replace('/[\\/.]+/', '.', $action);

	# Convert the action string to a path.
	$parts   = explode('.', $action);
	$parts[] = get_request_method() . '_' . array_pop($parts);
	$path    = './actions/' . implode('/', $parts) . '.inc';

	return $path;
}

/**
 *
 */
function self_url($params=array()) {
	$url = $_SERVER['PHP_SELF'];
	if (count($params) > 0) {
		$url .= '?' . build_query_string($params);
	}
	return $url;
}
?>
