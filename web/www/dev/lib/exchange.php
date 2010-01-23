<?php
/**
    @author Andres Blanc
    @license Whatever the Enlightenment Window Manager is using ATM.

    Fetches themes from http://exchange.enlightenment.org/ and provides several
    methods for displaying them.

    We should also improve the exchange side to add "sorting by" directly in the
    rss so we can use it with the already implmeneted limit parameter to not
    download the whole theme list

    @todo Implement a cache system (obsolete if exchange is updated)-
    @todo More functions with different focuses?
    @todo Implement sorting by rating in exchange.enlightenment.org because
          the server seems to limit the stream to simplexml_load_file for
          security (I hope) reasons.

*/
function themes ( $limit )
{
    static $themes = array ();

    if ( empty ($themes) )
        $themes = _themes_init(10);

    return array_keys(array_slice($themes, 0, $limit, true));
}

/**
    The resulting array of modules is similar to the array of themes instead
    it uses the module ID as key and te module name as value.
*/
function modules ( $limit )
{
    static $modules = array ();

    if ( empty ($modules) )
        $modules = _modules_init(10);

    return $modules;
}

# Helper for `themes()` generates the theme array from exchange.
function _themes_init ($limit)
{
    $themes = array ();
    if ( !$themes = cache("array.themes") )
    {
        $url = option('themes_url');
        $url = $url . "&limit=$limit";
        $xml = simplexml_load_file($url);

        foreach ($xml->themes->theme as $theme)
            $themes[(integer) $theme->id] = (float) $theme->rating;
        arsort($themes);

        cache("array.themes", $themes);
    }
    return $themes;
}

# Helper for `modules()` generates the module array from exchange.
function _modules_init ($limit)
{
    $modules = array ();
    if ( !$modules = cache("array.modules") )
    {
        $url = option('modules_url');
        $url = $url . "&limit=$limit";
        $xml = simplexml_load_file($url);

        $modules = array ();
        foreach ($xml->modules->module as $module)
            $modules[(integer) $module->id] = (string) $module->name;

        cache("array.modules", $modules);
    }
    return $modules;
}
?>

