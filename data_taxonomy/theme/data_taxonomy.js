Drupal.behaviors.data_taxonomy = function(context) {
  $('div.data-taxonomy-tagging-form:not(.data-taxonomy-processed)')
    .addClass('.data-taxonomy-processed')
    .each(function() {
      var tagging_form = $(this);
      $('a.data-taxonomy-edit', this).click(function() {
        $(tagging_form).addClass('data-taxonomy-editing');
        $('input.form-text', tagging_form).focus();
        return false;
      });
      $('input.form-text', this).keypress(function(event) {
        // Detect enter key.
        if (event.keyCode == 13) {
          $(tagging_form).removeClass('data-taxonomy-editing');
          return false;
        }
      });
      $('input.form-submit', this).mousedown(function() {
        $(tagging_form).removeClass('data-taxonomy-editing');
      });
    });
};
