<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * HTTP Utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 * Simple logging function; writes out all the arguments to a log file. This
 * is mainly meant for debugging or temporary monitoring purposes and should
 * *not* be left on on production sites.
 */
function logging() {
	global $cfg;
	if (!isset($cfg['LOG_FILE'])) {
		return;
	}

	$fp = fopen($cfg['LOG_FILE'], 'a');

	# Yes, this is a bit retarded, but it works fine for low to medium loads.
	while (!flock($fp, LOCK_EX)) {
		usleep(mt_rand() % 10);
	}

	$args = func_get_args();
	fwrite($fp, gmdate('Y-m-d H:i:s'));
	foreach ($args as $arg) {
		fwrite($fp, "\t" . $arg);
	}
	fwrite($fp, "\n");

	flock($fp, LOCK_UN);
	fclose($fp);
}
?>
