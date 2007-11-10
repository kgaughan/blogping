<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * HTML Helpers.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 * Yes, there are times when tables are ok for layout...
 */
class GridHelper {

	/** Number of columns in the grid. */
	var $columns;

	/** Current column in the grid. */
	var $i_column;

	/** Are we dealing with an open grid or not? */
	var $closed;

	/** Grid summary (for accessibility). */
	var $summary;

	/** CSS classes applied to the grid */
	var $classes;

	/**
	 * Initialises the grid helper.
	 *
	 * @param  $columns  Number of columns in the grid.
	 * @param  $summary  Summary of the grid contents (for accessibility).
	 * @param  $classes  CSS classes to apply.
	 */
	function GridHelper($columns, $summary='', $classes='') {
		$this->columns  = $columns;
		$this->i_column = 0;
		$this->closed   = true;
		$this->summary  = $summary;
		$this->classes  = $classes;
	}

	/**
	 * Start a new grid cell.
	 *
	 * @param  $force_wrap  Force the grid to wrap onto a new row.
	 */
	function cell($force_wrap=false) {
		if ($force_wrap) {
			$this->i_column = 0;
		}

		if ($this->i_column++ == $this->columns) {
			$this->i_column = 1;
		}

		if (!$this->closed) {
			echo "</td>\n";
			if ($this->i_column == 1) {
				echo "\t</tr>\n";
			}
		} else {
			$this->closed = false;
			echo '<table summary="', htmlentities($this->summary), '"';
			if ($this->classes != '') {
				echo ' class="', htmlentities($this->classes), '"';
			}
			echo "><tbody>\n";
		}

		if ($this->i_column == 1) {
			echo "\t<tr>\n";
		}
		echo "\t\t<td>";
	}

	/**
	 * Force the grid to wrap onto a new row.
	 */
	function wrap() {
		$this->i_column = 0;
	}

	/**
	 * Introduce a new grid cell that's logically part of the current one.
	 */
	function split_cell() {
		if (!$this->closed) {
			echo "</td>\n\t\t<td>";
		}
	}

	/**
	 * Close up the grid.
	 */
	function end_grid() {
		if (!$this->closed) {
			echo "</td>\n\t</tr>\n</tbody></table>";
		}
	}
}
?>
