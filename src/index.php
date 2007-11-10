<?php
/*
 * BlogPing
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Application-specific code.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

include './lib/antifwk.inc';

set_default('name', '');
set_default('url',  'http://');
set_default('ping', $cfg['DEFAULTS']);
save_params('name', 'url', 'ping');

$name = $_REQUEST['name'];
$url  = $_REQUEST['url'];

head();
include './templates/ping_form.inc';
foot();
?>
