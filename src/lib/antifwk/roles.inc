<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Role-based access control.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

session_start();

/**
 *
 */
function has_role($role) {
	return isset($_SESSION['roles']) && array_search($role, $_SESSION['roles']) !== false;
}

/**
 * Used on each page to enforce access roles
 */
function requires_role($role) {
	if (!has_role($role)) {
		global $cfg;
		header('HTTP/1.1 403 Forbidden');
		header('Location: ' . $cfg['BAD_ROLE_PAGE']);
		exit;
	}
}
?>
