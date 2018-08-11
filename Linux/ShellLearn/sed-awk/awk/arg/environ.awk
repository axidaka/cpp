# environ.awk -
BEGIN{
    for (env in ENVIRON)
        print env "=" ENVIRON[env]
}
