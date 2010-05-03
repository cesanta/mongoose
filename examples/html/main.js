// This file is part of Mongoose project, http://code.google.com/p/mongoose
// $Id: main.js 514 2010-05-03 11:06:27Z valenok $

var chat = {
  // Backend URL, string.
  // 'http://backend.address.com' or '' if backend is the same as frontend
  backendUrl: '',
  maxVisibleMessages: 10,
  errorMessageFadeOutTimeoutMs: 2000,
  errorMessageFadeOutTimer: null,
  lastMessageId: 0,
};

chat.refresh = function(data) {
  $.each(data, function(index, entry) {
    var row = $('<div>').addClass('message-row').appendTo('#mml');
    var timestamp = (new Date(entry.timestamp * 1000)).toLocaleTimeString();
    $('<span>').addClass('message-timestamp').html(
      '[' + timestamp + ']').prependTo(row);
    $('<span>').addClass('message-user').html(entry.user + ':').appendTo(row);
    $('<span>').addClass('message-text').html(entry.text).appendTo(row);
    chat.lastMessageId = Math.max(chat.lastMessageId, entry.id) + 1;
  });

  // TODO(lsm): keep only chat.maxVisibleMessages, delete older ones.
  /*
  while ($('#mml').children().length < chat.maxVisibleMessages) {
    $('#mml').children()[0].remove();
  }
  */
};

chat.getMessages = function() {
  $.ajax({
    dataType: 'jsonp',
    url: chat.backendUrl + '/ajax/get_messages',
    data: {last_id: chat.lastMessageId},
    success: chat.refresh,
    error: function() {
    },
  });
};

chat.handleMenuItemClick = function(ev) {
  $('.menu-item').removeClass('menu-item-selected');  // Deselect menu buttons
  $(this).addClass('menu-item-selected');  // Select clicked button
  $('.main').addClass('hidden');  // Hide all main windows
  $('#' + $(this).attr('name')).removeClass('hidden');  // Show main window
};

chat.showError = function(message) {
  $('#error').html(message).fadeIn('fast');
  window.clearTimeout(chat.errorMessageFadeOutTimer);
  chat.errorMessageFadeOutTimer = window.setTimeout(function() {
      $('#error').fadeOut('slow');
  }, chat.errorMessageFadeOutTimeoutMs);
};

chat.handleMessageInput = function(ev) {
  var input = ev.target;
  if (ev.keyCode != 13 || !input.value)
    return;
  input.disabled = true;
  $.ajax({
    dataType: 'jsonp',
    url: chat.backendUrl + '/ajax/send_message',
    data: {text: input.value},
    success: function(ev) {
      input.value = '';
      input.disabled = false;
      chat.getMessages();
    },
    error: function(ev) {
      chat.showError('Error sending message');
      input.disabled = false;
    },
  });
};

$(document).ready(function() {
  $('.menu-item').click(chat.handleMenuItemClick);
  $('.message-input').keypress(chat.handleMessageInput);
  chat.getMessages();
});

// vim:ts=2:sw=2:et
