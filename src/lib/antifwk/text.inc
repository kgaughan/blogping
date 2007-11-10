<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Text utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 *
 */
function magic_quotes($txt) {
	return  str_replace('"', '&#8221;',
			str_replace("'", '&#8217;',
			str_replace('&#8220;\'', '&#8220;&#8216;',
			preg_replace('/(^|\s|&#8216;)\"/', "\\1&#8220;",
			preg_replace('/(^|\s)\'/', "\\1&#8216;", $txt)))));
}

function latin1_to_utf8($txt) {
	# Logic cogged from Sam Ruby's article "Survival Guide to i18n" at
	# http://intertwingly.net/stories/2004/04/14/i18n.html
	return preg_replace('/([\x80-\xFF])/e',
		"ord('\\1') < 192 ? chr(194) . '\\1' : chr(195) . chr(ord('\\1') - 64)",
		$txt);
}

/**
 * Prepares text for output, escaping special characters and converting any
 * typewriter quotes to typographer's quotes, and converting it to UTF-8.
 */
function prepare_text($txt) {
	return magic_quotes(htmlspecialchars(latin1_to_utf8($txt), ENT_NOQUOTES));
}
?>
