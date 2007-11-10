<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Templating.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

# Requires markdown and ADODB.

/**
 *
 */
function get_page($name) {
	global $cfg;
	$db =& NewADOConnection($cfg['DB_DSN']);
	$rs =& $db->Execute('SELECT * FROM pages WHERE short_name = ?', array($name));
	if ($rs->EOF) {
		$result = array();
	} else {
		$result =& $rs->GetRowAssoc(false);
	}
	$rs->Close();
	$db->Close();
	return $result;
}

/**
 *
 */
function render_template($name) {
	if (file_exists($name)) {
		require './templates/' . $name;
	} else {
		$page = get_page(basename($name, '.inc'));
		if (count($page) > 1) {
			echo markdown($page['body']);
		}
	}
}

/**
 *
 */
function render_query_limit($qry, $template, $empty_template='', $n=-1, $offset=-1) {
	global $cfg;
	$db =& NewADOConnection($cfg['DB_DSN']);
	$rs =& $db->SelectLimit($qry, $n, $offset);
	render_each($rs, $template, $empty_template);
	$rs->Close();
	$db->Close();
}

/**
 *
 */
function render_each($_rows, $_template, $_empty_template='') {
	global $cfg;

	$record_count = 0;
	if (is_array($_rows)) {
		$record_count = count($_rows);
	} elseif ($_rows) {
		$record_count = $_rows->RecordCount();
	}

	if ($record_count == 0) {
		if ($_empty_template != '') {
			render_template($_empty_template);
		}
	} elseif (is_array($_rows)) {
		$current_row = 1;
		foreach ($_rows as $_row) {
			extract($_row);
			require './templates/' . $_template;
			++$current_row;
		}
	} else {
		$current_row = 1;
		while (!$_rows->EOF) {
			$_row = $_rows->GetRowAssoc(false);
			extract($_row);
			require './templates/' . $_template;
			++$current_row;
			$_rows->MoveNext();
		}
	}
}
?>
