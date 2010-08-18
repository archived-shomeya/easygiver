<div class="art-Post">
    <div class="art-Post-tl"></div>
    <div class="art-Post-tr"></div>
    <div class="art-Post-bl"></div>
    <div class="art-Post-br"></div>
    <div class="art-Post-tc"></div>
    <div class="art-Post-bc"></div>
    <div class="art-Post-cl"></div>
    <div class="art-Post-cr"></div>
    <div class="art-Post-cc"></div>
    <div class="art-Post-body">
<div class="art-Post-inner">

	<div class="comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) echo ' comment-unpublished'; ?>">
<div class="art-PostMetadataHeader">
		<h2 class="art-PostHeader"> 
			<?php if ($title) {echo $title; } ?>

		</h2>
		
		</div>
		
		<?php if ($submitted): ?>
			<span class="submitted"><?php echo $submitted; ?></span>
			<div class="cleared"></div>
		<?php endif; ?>	
		<?php if ($comment->new) : ?>
			<span class="new"><?php print drupal_ucfirst($new) ?></span>
		<?php endif; ?>
<div class="art-PostContent">
		
			<div class="art-article">
				<?php print $picture ?>
				<?php echo $content; ?>
			</div>

		</div>
		<div class="cleared"></div>
		
		<div class="links"><?php echo $links; ?><div class="cleared"></div></div>	
	</div>

</div>

    </div>
</div>
