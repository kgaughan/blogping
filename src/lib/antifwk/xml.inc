<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * XML Utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 * Wrapper around expat to convert an XML document to a struct/array.
 *
 * @param  $doc  XML document to parse.
 *
 * @return Array containing the parsed document.
 */
function parse_xml_into_struct($doc) {
	# Compensate from some odd behaviour in the parser.
	$doc = str_replace('&#32;', ' ', $doc);

	$p = xml_parser_create();
	xml_parser_set_option($p, XML_OPTION_SKIP_WHITE, TRUE);
	xml_parse_into_struct($p, $doc, $result);
	xml_parser_free($p);
	return $result;
}
?>
