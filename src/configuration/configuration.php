<?php
#
# Contains all the configuration details for your app. This file should contain
# no code which, if executed directly, could affect application state: Reading
# files, good; writing files, bad; using echo, print, &c., bad; assignment,
# good; writing to the database, bad; reading from the database, good; peeking
# at $_SESSION, good; changing $_SESSION, bad. You get the idea.
#

$cfg = array(
	# The name of your service.
	'APP_NAME' => 'Your BlogPing Service',

	# The aggregation services you're allowing people to ping using your
	# service.
	'RESPONDERS' => array(
		'wbc' => array(
			'name'      => 'Weblogs.com',
			'responder' => 'http://rpc.weblogs.com/RPC2',
			'url'       => 'http://weblogs.com/'
		),
		'rati' => array(
			'name'      => 'Technorati',
			'responder' => 'http://rpc.technorati.com/rpc/ping',
			'url'       => 'http://technorati.com/'
		),
		'bloglines' => array(
			'name'      => 'Bloglines',
			'responder' => 'http://bloglines.com/ping',
			'url'       => 'http://bloglines.com/'
		),
		'opml' => array(
			'name'      => 'Share Your OPML',
			'responder' => 'http://rpc.opml.org/RPC2',
			'url'       => 'http://share.opml.org/aggregator/'
		)
	),

	# Which aggregators are marked by default.
	'DEFAULTS' => array(
		'wbc', 'rati', 'bloglines'
	),

	# How many items should there be per row in the grid listing aggregation
	# services to ping.
	'AGGREGATOR_GRID_SIZE' => 3,

	# If present, any calls to logging() will log to this file.
	# 'LOG_FILE' => 'pings.log'
);
?>
