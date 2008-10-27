<?php
/*
 * BlogPing
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Copyright (c) Keith Gaughan, 2006-07. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

// Constants {{{

// I'd very much prefer if, when editing this software, you left this header
// as-is and neither altered nor deleted it.
define('APP_VERSION', "BlogPing/1.6.1");

define('APP_VERSION_FULL', APP_VERSION . " ({$_SERVER['HTTP_HOST']})");

// }}}

// Kill magic quotes {{{
if (ini_get('magic_quotes_gpc')) {
	fix_magic_quotes($_GET);
	fix_magic_quotes($_POST);
	fix_magic_quotes($_COOKIE);
	fix_magic_quotes($_REQUEST);
}
set_magic_quotes_runtime(0);
// }}}

// Configuration and Ping Responders {{{
define('SITE_NAME', 'Your BlogPing Service');

// Each responder must have a unique identifier, and you must specify a name,
// the URL of the site the responder sits on, and the URL of the responder
// itself.
$responders = array(
	'wbc' => array(
		'name'      => 'Weblogs.com',
		'responder' => 'http://rpc.weblogs.com/RPC2',
		'url'       => 'http://weblogs.com/'),
	'rati' => array(
		'name'      => 'Technorati',
		'responder' => 'http://rpc.technorati.com/rpc/ping',
		'url'       => 'http://technorati.com/'),
	'bloglines' => array(
		'name'      => 'Bloglines',
		'responder' => 'http://bloglines.com/ping',
		'url'       => 'http://bloglines.com/'),
	'fb' => array(
		'name'      => 'FeedBurner',
		'responder' => 'http://ping.feedburner.com/',
		'url'       => 'http://feedburner.com/'),
	'blogs' => array(
		'name'      => 'Blo.gs',
		'responder' => 'http://ping.blo.gs/',
		'url'       => 'http://blo.gs/'));
// }}}

// General Functions {{{

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
 *
 */
function ee($s) {
	echo htmlspecialchars($s, ENT_QUOTES, 'UTF-8');
}

/**
 *
 */
function e($s) {
	return htmlspecialchars($s, ENT_QUOTES, 'UTF-8');
}

// A simplistic implementation that's fine for my purposes.
if (!function_exists('http_build_query')) {
	function http_build_query($params) {
		$sep = '';
		$qs  = '';
		foreach ($params as $k => $v) {
			if (is_array($v)) {
				foreach ($v as $k1 => $v1) {
					$qs .= $sep . urlencode("{$k}[$k1]") . '=' . urlencode($v1);
					$sep = '&';
				}
			} else {
				$qs .= $sep . urlencode($k) . '=' . urlencode($v);
				$sep = '&';
			}
		}
		return $qs;
	}
}

// }}}

// Forms {{{

/**
 * Form helper to write out a text field.
 *
 * @param  $name   Name (and id) for the field.
 * @param  $value  Value of textbox; pulls the value from $_REQUEST if none
 *                 given.
 */
function textbox($name, $value=null) {
	if (is_null($value) && isset($_REQUEST[$name])) {
		$value = $_REQUEST[$name];
	}
	echo '<input type="text" name="', e($name), '" id="', e($name), '" value="', e($value), '">';
}

/**
 * Checks if a given checkbox/radiobutton in a scope is active or not.
 *
 * @param  $name   Name of the checkbox/radiobutton group to check.
 * @param  $value  Value of the checkbox if active.
 *
 * @return True if active, else false.
 */
function is_checked($name, $value) {
	return isset($_REQUEST[$name]) && array_search($value, $_REQUEST[$name]) !== false;
}

/**
 * Form helper generating a checkbox.
 *
 * @param  $name   Name of checkbox group.
 * @param  $value  Value corresponding to this checkbox.
 * @param  $label  Label text to use (automatically escaped).
 */
function checkbox($name, $value, $label) {
	echo '<label><input type="checkbox" name="', $name, '[]" ';
	if (is_checked($name, $value)) {
		echo 'checked="checked" ';
	}
	echo 'value="', e($value), '"> ', e($label), '</label>';
}

function get_responder_list($requested) {
	global $responders;

	$result = array();
	foreach ($requested as $k) {
		if (array_key_exists($k, $responders)) {
			$r = $responders[$k];
			$result[$r['name']] = $r['responder'];
		}
	}
	return $result;
}

// }}}

// Pinging {{{

/**
 * Performs an XML-RPC weblogUpdates.ping call to a given responder.
 *
 * @param  $responder  URL of the responder.
 * @param  $name       Name of your weblog.
 * @param  $url        The URL of it's homepage.
 *
 * @param  An array, the first field specifying whether it succeeded or not,
 *         and the second the message/error in the response.
 */
function ping($responder, $name, $url) {
	$request = build_ping($name, $url);
	list($success, $response) = send_ping($responder, $request);
	if ($success) {
		$vs = parse_ping($response);
		$success = $vs['flerror'] == 0;
		$response = $vs['message'];
	}
	return array($success, $response);
}

function build_ping($name, $url) {
	$name = e($name);
	$url  = e($url);

	// The two parameters are on the same line to compensate for a bug in
	// IrishBlog.ie and co.
	return <<<EOT
<?xml version="1.0" encoding="UTF-8"?>
<methodCall>
	<methodName>weblogUpdates.ping</methodName>
	<params>
		<param><value>$name</value></param><param><value>$url</value></param>
	</params>
</methodCall>
EOT;
}

function parse_ping($response) {
	// Compensate from some odd behaviour in the parser.
	$response = str_replace('&#32;', ' ', $response);

	$p = xml_parser_create();
	xml_parser_set_option($p, XML_OPTION_SKIP_WHITE, true);
	xml_parse_into_struct($p, $response, $values);
	xml_parser_free($p);

	// The weblogUpdates.ping spec isn't exactly compliant with the XML-RPC
	// specification: http://www.xmlrpc.com/weblogsCom. Rather than parsing
	// the response properly, munging the appropriate information is just
	// fine.
	$fields = array();
	$name = null;
	foreach ($values as $t) {
		if (isset($t['value'])) {
			if (is_null($name)) {
				$name = $t['value'];
			} else {
				$fields[$name] = $t['value'];
				$name = null;
			}
		}
	}
	return $fields;
}

function send_ping($responder, $body) {
	$p = parse_url($responder);
	$path = $p['path'];
	if (isset($p['query'])) {
		$path .= '?' . $p['query'];
	}
	$host = $p['host'];
	$port = isset($p['port']) ? $p['port'] : 80;

	$fp = @fsockopen($host, $port, $errno, $errstr, 5);
	if (!$fp) {
		return array(false, "Cannot connect");
	}

	// Using the alias for backward compatibility.
	socket_set_timeout($fp, 5);

	fwrite($fp, "POST $path HTTP/1.0\r\n");
	fwrite($fp, "Host: $host\r\n");
	fwrite($fp, "Date: " . gmdate('r') . "\r\n");
	fwrite($fp, "Connection: close\r\n");
	fwrite($fp, "Content-Length: " . strlen($body) . "\r\n");
	fwrite($fp, "Content-Type: text/xml; charset=UTF-8\r\n");
	fwrite($fp, "User-Agent: " . APP_VERSION_FULL . "\r\n\r\n");
	fwrite($fp, $body);

	$response = '';
	$content_length = null;
	$stage = 0;
	while (!feof($fp)) {
		$block = @fread($fp, 512);
		if ($block === false) {
			@fclose($fp);
			return array(false, 'Response broken');
		}

		$response .= $block;

		if ($stage == 2) {
			// Body.
			$content_length -= strlen($block);
			if ($content_length == 0) {
				break;
			}
		} else {
			// Headers.
			while ($stage < 2 && strpos($response, "\r\n") !== false) {
				list($line, $response) = explode("\r\n", $response, 2);
				if ($stage == 0) {
					// Status line.
					list($version, $status, $reason) = explode(' ', $line, 3);
					// TODO: Cope with redirection.
					if ($status < 200 && $status >= 300) {
						@fclose($fp);
						return array(false, "HTTP: $status $reason");
					}
					$stage++;
				} elseif ($line === '') {
					// Header/body separator.
					$stage++;
					break;
				} else {
					// Header line.
					list($name, $value) = explode(':', $line, 2);
					if (strtolower($name) == 'content-length') {
						$content_length = intval($value);
					}
				}
			}
		}
	}
	@fclose($fp);

	return array(true, $response);
}

// }}}

// Ping Programmatically {{{
function ping_programmatically() {
	global $responders;
	$errors = array();
	if (trim($_POST['name']) == '') {
		$errors[] = 'No weblog name.';
	}
	if (trim($_POST['url']) == '') {
		$errors[] = 'No URL.';
	}
	if (!isset($_POST['ping'])) {
		$keys = implode(', ', array_keys($responders));
		$errors[] = "No services specified. Valid keys are: $keys.";
	}
	header('Content-Type: text/plain; charset=utf-8', true, count($errors) == 0 ? 200 : 400);
	if (count($errors) > 0) {
		echo implode("\n", $errors), "\n";
	} else {
		foreach (get_responder_list($_POST['ping']) as $name => $responder) {
			list($success, $msg) = ping($responder, $_POST['name'], $_POST['url']);
			$msg = str_replace(array("\n", "\t"), array('\n', '\t'), $msg);
			echo $success ? '+' : '-', "\t$name\t$msg\n";
		}
	}
}
// }}}

// Page Template {{{
function page_template() {
	global $responders;
	header('Content-Type: text/html; charset=utf-8');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN">

<html lang="en"><head>

	<link rel="stylesheet" href="assets/screen.css" type="text/css" media="screen">
	<meta name="Powered by" content="<?php ee(APP_VERSION) ?>">
	<title><?php ee(SITE_NAME) ?></title>

</head><body>

<h1><?php ee(SITE_NAME) ?></h1>

<div id="wrapper">
<form method="post" action="<?php ee($_SERVER['PHP_SELF']) ?>">

<p><label for="name">Your weblog&rsquo;s name</label>
<?php textbox('name') ?></p>

<p><label for="url">Your weblog&rsquo;s URL</label>
<?php textbox('url') ?></p>

<fieldset>
<legend>Services to ping</legend>
<ul id="responders">
<?php foreach ($responders as $k => $r) { ?>
	<li><?php checkbox('ping', $k, $r['name']) ?>
		<a href="<?php ee($r['url']) ?>" title="<?php ee($r['name']) ?>"><img src="assets/images/outward.png" height="16" width="20" alt="External Link"></a></li>
<?php } ?>
</ul>
</fieldset>

<p><input type="submit" value="Ping!"></p>

<?php if ($_SERVER['REQUEST_METHOD'] == 'POST') { ?>
	<ul id="ping-results">

	<?php if (trim($_POST['name']) == '') { ?>
		<li class="failure">You didn&rsquo;t provide a weblog name!</li>
	<?php } elseif (trim($_POST['url']) == '') { ?>
		<li class="failure">You didn&rsquo;t provide your weblog&rsquo;s URL!</li>
	<?php } elseif (!isset($_POST['ping'])) { ?>
		<li class="failure">You didn&rsquo;t select any services!</li>
	<?php } else { ?>
		<?php foreach (get_responder_list($_POST['ping']) as $name => $responder) { ?>
			<?php @flush(); list($success, $msg) = ping($responder, $_POST['name'], $_POST['url']) ?>
			<li class="<?php echo $success ? 'success' : 'failure' ?>">
				<strong><?php ee($name) ?></strong>: <?php ee($msg) ?>
			</li>
		<?php } ?>
	<?php } ?>

	</ul>
	<p>Bookmark this link to save your settings:
	<a href="?<?php ee(http_build_query($_POST)) ?>">BlogPing for <?php ee($_POST['name']) ?></a></p>
<?php } ?>

</form>

</div>

<address>
<a href="http://blogping.sourceforge.net/" title="Download the source code"><?php ee(APP_VERSION) ?></a> is
Copyright &copy; <a href="http://talideon.com/">Keith Gaughan</a>, 2006&ndash;07.<br>
Have any suggestions? <a href="http://talideon.com/about/contact/">Tell me</a>.
</address>

</body></html>
<?php
}
// }}}

// I'd very much prefer if, when editing this software, you left this header
// as-is and neither altered nor deleted it.
header('X-Powered-By: ' . APP_VERSION_FULL);
if ($_SERVER['REQUEST_METHOD'] == 'POST' && array_key_exists('quiet', $_POST)) {
	// If somebody wants to use it programmatically.
	ping_programmatically();
} else {
	page_template();
}
