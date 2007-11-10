<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * A PHP antiframework.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

# Include the core parts of Antifwk bundled with this app.
foreach (glob(dirname(__FILE__) . "/antifwk/*.inc") as $f) {
	include $f;
}

# Fix superglobals.
fix_superglobals();

# Configuration file, if any.
if (file_exists('./configuration/configuration.php')) {
	include './configuration/configuration.php';
}

# Shared libraries.
if (file_exists('./shared.inc')) {
	include './shared.inc';
}

$action = get_action_path();
if (file_exists($action)) {
	include $action;
}
?>
