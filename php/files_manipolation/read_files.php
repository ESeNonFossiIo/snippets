<?php

  /**
   * Questa funzione prende il nome del file da leggere e restituisce un testo
   * pari pari a quello letto.
   * @param  [type]  $file_name  filename che contiente il testo da leggere
   * @param  boolean $html_style abilita il formato HTML
   * @return [type]              testo letto
   */
  function read_from_file($file_name, $html_style = false)
  {
    $var    = "";
    $myfile = fopen($file_name, "r")
              or die("Unable to open file!");

    if($html_style == false)
    {
      $var = fread($myfile, filesize($file_name));
    }
    else
    {
      while(!feof($myfile))
      {
        $var .= fgets($myfile) . "<br>";
      }
    }

    fclose($myfile);
    return $var;
  }
?>
