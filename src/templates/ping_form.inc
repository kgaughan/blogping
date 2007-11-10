<?php form() ?>

<div style="margin-top:1ex">
	<strong><label for="name">Your weblog&rsquo;s name:</label></strong><br/>
	<?php textbox('name') ?>
</div>

<div style="margin-top:1ex">
	<strong><label for="url">Your weblog&rsquo;s URL:</label></strong><br/>
	<?php textbox('url') ?>
</div>

<div style="margin-top:1ex">
	<strong>Ping:</strong><br/>
	<?php ping_checkbox_grid() ?>
</div>

<div style="margin-top:1ex">
	<input type="submit" id="submit" value="Ping!" style="float:right;margin-left:1em"/>
	The settings you provide will be remembered so you don&rsquo;t have to
	retype them.
</div>

<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	include './templates/service_ping.inc';
}
?>
</form>

