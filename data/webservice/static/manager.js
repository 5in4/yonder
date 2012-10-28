$(document).ready(function() {
    $.each(["hotkeys", "atmosphere", "singleshots", "sfx"], function(ind, cat) {
        $.getJSON('json?manager=' + cat, function(data) {
            $.each(data, function(index, value) {
                $('#' + cat + '-list').append('<label><input class="' + cat + '" id="' + cat + index + '" name="" type="checkbox" data-pos="' + index + '" />' + value + '</label>');
            });

            $('#' + cat + '-list').trigger('create');

            $('.' + cat).change(function() {
                if($(this).prop('checked') == true) {
                    $.get('/button/' + cat, {pos: $(this).attr('data-pos'), state: "checked"}, function(){});
                }

                if($(this).prop('checked') == false) {
                    $.get('/button/' + cat, {pos: $(this).attr('data-pos'), state: "unchecked"}, function(){});
                }
            });

        });

    });

    setState();
    setInterval('setState()', $('body').attr('data-update-interval'));
});

function setState() {
    $.each(["hotkeys", "atmosphere", "singleshots", "sfx"], function(ind, cat) {
        $.getJSON('json-state?manager=' + cat, function(data) {
            $.each(data, function(index, value) {
                state = false;
                if(value == 1) {
                    state = true;
                }
                $('#' + cat + index).attr('checked', state).checkboxradio('refresh');
            });
        });
    });
}
