  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_dash_init(&mgr);

  while (1)
  {
    mg_mgr_poll(&mgr, 1);
    mg_dash_poll(&mgr);
