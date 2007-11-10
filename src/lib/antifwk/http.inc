<?php
/*
 * Antifwk
 * by Keith Gaughan <hereticmessiah@users.sourceforge.net>
 *
 * HTTP Utilities.
 *
 * Copyright (c) Keith Gaughan, 2006. All Rights Reserved.
 * See 'LICENSE' file for license details.
 */

define('HTTP_TIMEOUT', 2);

/**
 * A simple HTTP client.
 *
 * @param  $verb    HTTP verb (GET, POST, HEAD, &c.) to use for the request.
 * @param  $url     URL to make the request to.
 * @param  $body    Body of request; if an array, it's converted into formdata.
 * @param  $req_hs  Request headers.
 *
 * @return An array containing the key 'ERROR' if an error occurred, and the
 *         keys 'HEADERS' and 'BODY' if the request could be partly or fully
 *         fulfilled. 'ERROR' give an error message; 'HEADERS', an array of
 *         headers; and 'BODY', the response body.
 */
function http_do($verb, $url, $body='', $req_hs=array()) {
	$parts = parse_url($url);
	if (!isset($parts['port'])) {
		$parts['port'] = 80;
	}

	$fp = @fsockopen($parts['host'], $parts['port'], $errno, $errstr, HTTP_TIMEOUT);
	if (!$fp) {
		return array('ERROR' => 'Cannot connect: ' . $errstr);
	}
	stream_set_timeout($fp, HTTP_TIMEOUT);

	$qs = $parts['path'];
	if (isset($parts['query'])) {
		$qs .= '?' . $parts['query'];	
	}

	$req_hs['Host']       = $parts['host'];
	$req_hs['Connection'] = 'close';
	$req_hs['Date']       = gmdate('r');

	# So that you can pass in form data as an array.
	if (is_array($body)) {
		$body = build_query_string($body);
		$req_hs['Content-Type'] = 'application/x-www-form-urlencoded';
	}
	$req_hs['Content-Length'] = strlen($body); # Future Unicode problems?

	# Build and sent the request. We're using HTTP/1.0 to make sure the reply
	# doesn't specify "Transfer-Encoding: chunked", which we can't as yet cope
	# with: we can't cope with enough of HTTP/1.1 to say we can.
	$req = strtoupper($verb) . ' '  . $qs . ' HTTP/1.0' . CRLF;
	foreach ($req_hs as $name=>$value) {
		$req .= $name . ': ' . $value . CRLF;
	}
	$req .= CRLF . $body;
	fwrite($fp, $req);

	# Grab the start time of the request so we can check if the server's
	# timed out.
	list(, $start_sec) = explode(' ', microtime(false));

	$res_hs           = array();
	$res              = '';
	$status_line_read = false;
	$headers_read     = false;
	$content_length   = null;
	while (!feof($fp)) {
		$block = @fread($fp, 2048);
		if ($block === false) {
			@fclose($fp);
			return array(
				'ERROR'   => 'Response broken',
				'HEADERS' => $res_hs,
				'BODY'    => $res
			);
		}

		# We received some data, so we reset the timeout
		if (strlen($block) > 0) {
			$res .= $block;
			list(, $start_sec) = explode(' ', microtime(false));
		}

		if (!is_null($content_length) && strlen($res) == $content_length) {
			# We've received the full response.
			break;
		}

		if (!$headers_read) {
			# Process any header lines we've read.
			while (strpos($res, CRLF) !== false) {
				list($line, $res) = explode(CRLF, $res, 2);

				if (!$status_line_read) {
					# Check if the response is good.
					$status_line_read = true;
					if (strpos($line, '200') === false) {
						return array('ERROR' => 'Response not OK');
					}
				} else if ($line == '') {
					# Finished processing headers: process body.
					$headers_read = true;
					if (isset($req_hs['content-length'])) {
						$content_length = $req_hs['content-length'];
					}
					break;
				} else if (strspn($line, " \t") > 0 && isset($name)) {
					# Lines starting with whitespace get appended to the
					# previous header line. [RFC2616:4.2]
					$res_hs[$name] .= ' ' . trim($line);
				} else {
					# Process the header line.
					list($name, $value) = explode(':', $line, 2);
					$name = strtolower($name);
					if (isset($res_hs[$name])) {
						# Multiple headers with same name: normalise it to a
						# comma-seperated list. [RFC2616:4.2]
						$res_hs[$name] .= ',' . trim($value);
					} else {
						$res_hs[$name] = trim($value);
					}
				}
			}
		}

		# Check if the server's taking too long to respond with data.
		list(, $sec) = explode(' ', microtime(false));
		if ($sec - $start_sec >= HTTP_TIMEOUT) {
			fclose($fp);
			return array(
				'ERROR'   => 'Request sent, but server took too long to respond',
				'HEADERS' => $res_hs,
				'BODY'    => $res
			);
		}
	}

	fclose($fp);
	return array('HEADERS' => $res_hs, 'BODY' => $res);
}

function http_get($url, $args) {
	if (count($args) > 0) {
		$url .= '?' . build_query_string($args);
	}
	return http_do('GET', $url);
}
?>
