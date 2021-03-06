<?php
require_once 'Common.inc';

/**
 * List size operation
*/

class ListMerge extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "list_merge");
        $this->put_record = array("John", 15, array(25, 45), array("name"=>"Dan"), 45.8, True);
        $this->db->put($key,array("binA"=>100, "binB" => $this->put_record));
        $this->keys[] = $key;
    }

    public function generateRandomString($length = 10) {
        $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        $charactersLength = strlen($characters);
        $randomString = '';
        for ($i = 0; $i < $length; $i++) {
            $randomString .= $characters[rand(0, $charactersLength - 1)];
        }
        return $randomString;
    }

    /**
     * @test
     * List merge operation with all positive arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_001)
     *
     * @test_plans{1.1}
     */
    function normal_001() {
        $items = array(10, "aa", 12.856, True, array("I1"=>"bags"), array(500, 5000));
        $status = $this->db->listMerge($this->keys[0], "binB", $items,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array_merge($this->put_record, $items));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List merge operation with items is list of strings.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_002)
     *
     * @test_plans{1.1}
     */
    function normal_002() {
        $items = array("Aerospike", "Casandra", "Redis");
        $status = $this->db->listMerge($this->keys[0], "binB", $items);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array_merge($this->put_record, $items));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * List merge operation with items is an empty list.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_003)
     *
     * @test_plans{1.1}
     */
    function normal_003() {
        $items = array();
        $status = $this->db->listMerge($this->keys[0], "binB", $items);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array_merge($this->put_record, $items));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List merge operation with non-existent bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_004)
     *
     * @test_plans{1.1}
     */
    function normal_004() {
        $items = array("Aerospike", "Casandra", "Redis");
        $bin = $this->generateRandomString();
        $status = $this->db->listMerge($this->keys[0], $bin, $items);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array($bin));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"][$bin], $items);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List get range operation with non-existent key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (error_001)
     *
     * @test_plans{1.1}
     */
    function normal_005() {
        $items = array("Aerospike", "Casandra", "Redis");
        $key = $this->db->initKey("test", "demo", $this->generateRandomString());
        $this->keys[] = $key;
        $status = $this->db->listMerge($key, "binC", $items);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($key, $get_record);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binC"],$items);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List merge operation with key parameter is of type an integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_001)
     *
     * @test_plans{1.1}
     */
    function edge_001() {
        $items = array("Aerospike", "Casandra", "Redis");
        try {
			$status = $this->db->listMerge(200, "binB", $items);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}

			return Aerospike::ERR_CLIENT;
        } catch (ErrorException $e) {
        	return $this->db->errorno();
        }
    }
    
    /**
     * @test
     * List merge operation with bin is of type an array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_002)
     *
     * @test_plans{1.1}
     */
    function edge_002() {
        $items = array("Aerospike", "Casandra", "Redis");
        try {
            $status = $this->db->listMerge($this->keys[0], array(599), $items);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List merge operation with items parameter is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_003)
     *
     * @test_plans{1.1}
     */
    function edge_003() {
        $items = "string";
        try {
            $status = $this->db->listMerge($this->keys[0], "binB", $items);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List merge operation with options parameter is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_004)
     *
     * @test_plans{1.1}
     */
    function edge_004() {
        $items = array("Aerospike", "Casandra", "Redis");
        try {
            $status = $this->db->listMerge($this->keys[0], "binB", $items, "abcdef");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
}
?>
