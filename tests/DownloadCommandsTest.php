<?php
/**
 * @file
 * PHPUnit tests for Backdrop Console Download commands.
 */

use PHPUnit\Framework\TestCase;

class DownloadCommandsTest extends TestCase {

  /**
   * Make sure that the download command works.
   */
  public function test_download_command_works() {
    // Single module.
    $output_single = shell_exec('b download --hide-progress simplify');
    $this->assertStringContainsString("'simplify' was downloaded into '/app/backdrop/modules/simplify'.", $output_single);
    $this->assertTrue(file_exists('/app/backdrop/modules/simplify/simplify.info'));

    // Multiple projects (theme and layout).
    $output_multiple = shell_exec('b download --hide-progress lumi bamboo');
    $this->assertStringContainsString("'lumi' was downloaded into '/app/backdrop/themes/lumi'.", $output_multiple);
    $this->assertTrue(file_exists('/app/backdrop/themes/lumi/lumi.info'));
    $this->assertStringContainsString("'bamboo' was downloaded into '/app/backdrop/layouts/bamboo'.", $output_multiple);
    $this->assertTrue(file_exists('/app/backdrop/layouts/bamboo/bamboo.info'));

    // Cleanup downloads.
    exec('rm -fr /app/backdrop/modules/simplify /app/backdrop/themes/lumi /app/backdrop/layouts/bamboo');
  }

  /**
   * Make sure that the download-core command works.
   */
  public function test_download_core_command_works() {
    // Download to current directory.
    $output_current = shell_exec('mkdir /app/current && cd /app/current && b download-core --hide-progress');
    $this->assertStringContainsString("Backdrop was downloaded into '/app/current'.", $output_current);
    $this->assertTrue(file_exists('/app/current/index.php'));

    // Download to specified directory.
    $output_directory = shell_exec('b download-core --hide-progress /app/directory');
    $this->assertStringContainsString("Backdrop was downloaded into '/app/directory'.", $output_directory);
    $this->assertTrue(file_exists('/app/directory/index.php'));

    // Cleanup downloads.
    exec('rm -fr /app/current /app/directory');
  }

}