<?php
/**
 * Generates a grid of checkboxes listing aggregations services we act as
 * a proxy to.
 */
function ping_checkbox_grid() {
	global $cfg;

	$grid = new GridHelper($cfg['AGGREGATOR_GRID_SIZE'], '', 'checkboxes');
	foreach ($cfg['RESPONDERS'] as $id=>$props) {
		$grid->cell(isset($props['break']) && $props['break']);
		checkbox('ping', $id, $props['name']);
		$grid->split_cell();
		echo '<a href="', htmlentities($props['url']), '" title="';
		echo htmlentities($props['name']), '">link</a>';
	}
	$grid->end_grid();
}
?>
