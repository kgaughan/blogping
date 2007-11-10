<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Time Utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 *
 */
function edate($format, $dt) {
	if (!is_int($dt)) {
		$dt = strtotime($dt);
	}
	echo strftime($format, $dt);
}

function iso8601_datetime($dt) {
	if (!is_int($dt)) {
		$dt = strtotime($dt);
	}
	return date('Y-m-d\TH:i:sO', $dt);
}

/**
 * Generates a timespan in seconds.
 *
 * @param  $sec  Number of seconds to include in timespan; may exceed 60.
 * @param  $min  Ditto for minutes.
 * @param  $hr   Ditto for hours; may exceed 24.
 * @param  $dy   Ditto for days.
 *
 * @return Specified timespan in seconds.
 */
function make_timespan($sec=0, $min=0, $hr=0, $dy=0) {
	return $sec + ($min * 60) + ($hr * 360) + ($dy * 8640);
}
?>
