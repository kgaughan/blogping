<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Blogging utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 * Performs an XML-RPC weblogUpdates.ping call to a given responder.
 *
 * @param  $responder  URL of the responder.
 * @param  $name       Name of your weblog.
 * @param  $url        The URL of it's homepage.
 * @param  $ua         The name of your blogging engine.
 *
 * @param  An array, the first field specifying whether it succeeded or not,
 *         and the second the message/error in the response.
 */
function ping($responder, $name, $url, $ua='Antifwk/1.0') {
	$name = htmlspecialchars($name, ENT_NOQUOTES);
	$url  = htmlspecialchars($url,  ENT_NOQUOTES);

	#
	# In a sane world, this would look like:
	#
	# <call method="weblogUpdates.ping">
	#     <string>$name</string>
	#     <string>$url</string>
	# </call>
	#
	# But that would have been far too sensible...
	#

	# The two parameters are on the same line to compensate for a bug in
	# IrishBlog.ie and co.
	$body = <<<EOT
<?xml version="1.0" encoding="UTF-8"?>
<methodCall>
	<methodName>weblogUpdates.ping</methodName>
	<params>
		<param><value>$name</value></param><param><value>$url</value></param>
	</params>
</methodCall>
EOT;

	$res = http_do('POST', $responder, $body, array(
		'Content-Type' => 'text/xml; charset=UTF-8',
		'User-Agent'   => $ua
	));
	if (isset($res['ERROR'])) {
		return array(false, $res['ERROR']);
	}

	# dump($res['BODY']);

	# The weblogUpdates.ping spec isn't exactly compliant with the XML-RPC
	# specification: http://www.xmlrpc.com/weblogsCom. Rather than parsing the
	# response properly, munging the appropriate information is just fine.
	$flerror = 0;
	$message = '';
	$name    = '';
	foreach (parse_xml_into_struct($res['BODY']) as $t) {
		if (isset($t['value'])) {
			if ($name == '') {
				$name = $t['value'];
			} else {
				if ($name == 'flerror') {
					$flerror = intval($t['value']);
				} elseif ($name == 'message') {
					$message = $t['value'];
				}
				$name = '';
			}
		}
	}

	return array($flerror == 0, $message);
}
?>
