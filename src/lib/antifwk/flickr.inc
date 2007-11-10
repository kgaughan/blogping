<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * Flickr Utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

/**
 *
 */
function flickr_get_photos_for_tag($api_key, $tag, $page=1, $per_page=20) {
	$response = http_get('http://flickr.com/services/rest/', array(
		'method'   => 'flickr.photos.search',
		'api_key'  => $api_key,
		'tags'     => $tag,
		'per_page' => $per_page,
		'page'     => $page
	));

	if (isset($response['ERROR'])) {
		return false;
	}

	$struct = parse_xml_into_struct($response['BODY']);

	$pages = 0;
	$total = 0;
	$items = array();
	foreach ($struct as $elem) {
		switch ($elem['tag']) {
			case 'PHOTOS':
				if ($elem['type'] == 'open') {
					$a     = $elem['attributes'];
					$pages = $a['PAGES'];
					$total = $a['TOTAL'];
				}
				break;

			case 'PHOTO':
				$a      = $elem['attributes'];
				$server = $a['SERVER'];
				$id     = $a['ID'];
				$secret = $a['SECRET'];

				$items[] = array(
					'image' => "http://static.flickr.com/$server/{$id}_{$secret}.jpg",
					'small' => "http://static.flickr.com/$server/{$id}_{$secret}_s.jpg",
					'url'   => "http://flickr.com/photos/{$a['OWNER']}/$id/",
					'title' => $a['TITLE']
				);
				break;
		}
	}

	return array(
		'page'  => $page,
		'of'    => $pages,
		'total' => $total,
		'items' => $items
	);
}
?>
