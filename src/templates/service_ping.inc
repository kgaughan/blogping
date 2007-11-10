<table summary="" id="ping-results"><tbody>
<?php
logging('pinging', implode(',', $_REQUEST['ping']), $url, $name);
foreach ($_REQUEST['ping'] as $id) {
	$r = $cfg['RESPONDERS'][$id];
?>
	<tr>
		<th><?php eecho($r['name']) ?></th>
<?php
	@flush();
	list($successful, $message) =
		ping($r['responder'], $name, $url,
			'BlogPing/1.1 (' . $cfg['APP_NAME'] .
			'; http://' . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] . ')');
?>
		<td class="<?php echo $successful ? 'success' : 'failure' ?>">
			<?php eecho($message) ?>
		</td>
	</tr>
<?php
}
?>
</tbody></table>

