<?php
/**
 * @file
 * Open Graph node rendered preview.
 *
 * Open graph object type is also used for this template suggestions:
 *
 *   oconsumer-preview--TYPE.tpl.php
 *
 * will always be available (you need to copy this file first into your
 * theme for this to work, sad Drupal is sad).
 *
 * Available variables are quite clear, note that all of them can
 * be null (sorry, open graph is not something very serious in real
 * life):
 *  - $image_url : Image URI if one provided
 *  - $image_height : Image height.
 *  - $image_width : Image width
 *  - $title : safe title
 *  - $description : safe description
 *  - $site_name : safe incomming site name
 *  - $author : safe author name
 *  - $url : original URL
 *  - $type : "og:type" prefix
 *
 * Some others will come.
 *
 * You also can use:
 *  - $node : \OgConsumer\Object instnace, you can use this if you don't want
 *    to write your own theme preprocessor for this template.
 */
?>
<div class="preview">
  <a href="<?php echo $url; ?>">
    <?php if ($image_url): ?>
    <div class="image">
      <!--
          HUGE FIXME: Copy and resize images locally.
      <?php if ($image_height): ?>
      <img src="<?php echo $image_url; ?>"
         heigth="<?php echo $image_height; ?>"
         width="<?php echo $image_width; ?>"
         alt=""/>
      <?php else: ?>
      -->
      <img src="<?php echo $image_url; ?>" alt=""/>
      <?php endif; ?>
    </div>
    <?php endif; ?>
    <div>
      <h3>
        <?php echo $title; ?>
      </h3>
      <?php if ($description): ?>
      <cite class="description">
        <?php echo $description; ?>
      </cite>
    <?php endif; ?>
    </div>
    <div class="footer"></div>
  </a>
</div>